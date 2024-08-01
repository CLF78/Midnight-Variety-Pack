#include "DriftTiers.hpp"
#include <egg/effect/eggEffectResource.hpp>
#include <game/effect/EffectsManager.hpp>
#include <game/kart/KartBoost.hpp>
#include <game/kart/KartMove.hpp>
#include <game/kart/KartObjectManager.hpp>
#include <game/kart/KartStats.hpp>
#include <game/sound/AudioHandleHolder.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <nw4r/snd/SoundArchivePlayer.hpp>

static EGG::EffectResource* pulEffects = nullptr;
static EGG::EffectResource* vpEffects = nullptr;

const char* ExpPlayerEffects::UMTNames[8] = {
    "rk_driftSpark3L_Spark00",
    "rk_driftSpark3L_Spark01",
    "rk_driftSpark3R_Spark00",
    "rk_driftSpark3R_Spark01",
    "rk_purpleTurbo",
    "rk_purpleTurbo",
    "rk_purpleTurbo",
    "rk_purpleTurbo"
};

const char* ExpPlayerEffects::SMTNames[8] = {
    "rk_driftSpark2L_Spark00",
    "rk_driftSpark2L_Spark01",
    "rk_driftSpark2R_Spark00",
    "rk_driftSpark2R_Spark01",
    "rk_orangeTurbo",
    "rk_orangeTurbo",
    "rk_orangeTurbo",
    "rk_orangeTurbo"
};

//Needed so that other players display the correct effect
bool umtState[12]; //false = no UMT  true = UMT buff active expanding Kart::Movement just for this doesn't seem like the plan

//kmWrite32(0x8057ee5c, 0x2c050004); //changes >= 3 to >= 4 for UMT
//kmWrite32(0x8057ef30, 0x2c000001); //changes check from if != 2 to if = 1, so that when in a SMT the function keeps going
//kmWrite32(0x8057ef38, 0x418200A4); //ensure mtSpeedMultiplier gets reset when driftState = 1, by sending to where SetKartDriftTiers hooks
//kmWrite32(0x8057efb4, 0x48000028); //skips the SMT charge check and sends unconditionally to SetKartDriftTiers
//void SetKartDriftTiers(KartMove& movement) {
//    int type = movement.getDriftType();
//    const s16 smtCharge = movement.smtCharge;
//    if (type == KartStats::KART) {
//        if (smtCharge >= 550) movement.driftState = 4;
//        else if (smtCharge >= 300) movement.driftState = 3;
//    }
//    else if (type == KartStats::INSIDE_BIKE && smtCharge >= 300) movement.driftState = 3;
//};
//kmBranch(0x8057efdc, SetKartDriftTiers);
//
//kmWrite32(0x80588894, 0x2c000003); //changes >= 2 to >= 3 for SMT
//kmWrite32(0x8058889c, 0x2c000003); //changes check from if != 1 to if = 3, so that when in a MT the function keeps going
//kmWrite32(0x805888a8, 0x418200A4); //if in charge state 2, skip to SetBikeDriftTiers
//kmWrite32(0x80588928, 0x60000000); //removed fixed 270 write to mtCharge
//kmWrite32(0x80588938, 0x7c040378); //setup "charged" for next function
//kmWrite32(0x8058893c, 0x48000010); //takes MT charge check and parses it into SetBikeDriftTiers
//void SetBikeDriftTiers(KartMove& movement, bool charged) {
//    if (charged) {
//        movement.driftState = 2;
//        int type = movement.getDriftType();
//        const s16 mtCharge = movement.mtCharge;
//        if (type == KartStats::OUTSIDE_BIKE) {
//            if (mtCharge >= 570) movement.driftState = 3;
//        }
//    }
//}
//kmBranch(0x8058894c, SetBikeDriftTiers);

//Buffs MTStats and updates umtState
//int BuffUMT(KartMove& movement) {
//    u8 idx = movement.getPlayerIdx();
//    u32 mtStat = movement.getStats().mtDuration;
//    bool* state = umtState;
//    if (movement.driftState == 4) state[idx] = true;
//    if (state[idx] == true) mtStat = 3 * mtStat / 2; //50% longer
//    return mtStat;
//};
//kmCall(0x80582fdc, BuffUMT);
//kmWrite32(0x80582fe0, 0x7C601B78);
//kmWrite32(0x80582fec, 0x4180003C); //changes !=3 to <3 for UMT
//
////SpeedMultiplier "perk" implementation
//bool UpdateSpeedMultiplier(KartBoost& boost, bool* boostEnded) {
//    const bool isBoosting = boost.calc(boostEnded);
//    register KartMove* movement;
//    asm(mr movement, r28;);
//    const u8 id = movement->getPlayerIdx();
//    bool* state = umtState;
//    const float umtMultiplier = 1.32f; //10% faster
//    const float defaultMTMultiplier = 1.2f;
//
//    if (!isBoosting) state[id] = false;
//    if (boost.boostMultiplier == defaultMTMultiplier || boost.boostMultiplier == umtMultiplier) {
//        if (state[id]) boost.boostMultiplier = umtMultiplier;
//        else boost.boostMultiplier = defaultMTMultiplier;
//    }
//    return isBoosting;
//}
//kmCall(0x8057934c, UpdateSpeedMultiplier);

//Expanded player effect, also hijacked to add custom breff/brefts to EffectsMgr
static void CreatePlayerEffects(EffectsManager& mgr) { //adding the resource here as all other breff have been loaded at this point
    ResourceManager* root = ResourceManager::instance;
    void* breff = root->getFile(MultiDvdArchive::COMMON, "/Effect/purple.breff", 0);
    void* breft = root->getFile(MultiDvdArchive::COMMON, "/Effect/purple.breft", 0);
    EGG::EffectResource* res = new EGG::EffectResource(breff, breft);
    if (mgr.resCount != 9) mgr.resources[mgr.resCount] = res;
    else pulEffects = res;
    breff = root->getFile(MultiDvdArchive::COMMON, "/Effect/orange.breff", 0);
    breft = root->getFile(MultiDvdArchive::COMMON, "/Effect/orange.breft", 0);
    res = new EGG::EffectResource(breff, breft);
    if (mgr.resCount != 9) mgr.resources[mgr.resCount] = res;
    else vpEffects = res;

    for(int i = 0; i < RaceConfig::instance->raceScenario.playerCount; ++i) {
        mgr.players[i] = new(ExpPlayerEffects)(KartObjectManager::instance->getKartObject(i));
    }
}
kmCall(0x80554624, CreatePlayerEffects);

static void DeleteEffectRes(EffectsManager& mgr) {
    delete(pulEffects);
    pulEffects = nullptr;
    delete(vpEffects);
    vpEffects = nullptr;
    mgr.Reset();
}
kmCall(0x8051b198, DeleteEffectRes);


//Loads the custom effects
static void LoadCustomEffects(ExpPlayerEffects& effects) {
    effects.loadEffects();
    effects.rk_purpleMT = new EGG::Effect * [ExpPlayerEffects::UmtEffectsCount];
    for(int i = 0; i < ExpPlayerEffects::UmtEffectsCount; ++i) {
        effects.rk_purpleMT[i] = new(EGG::Effect)(ExpPlayerEffects::UMTNames[i], effects.playerIdPlus2);
    }
    effects.rk_orangeMT = new EGG::Effect * [ExpPlayerEffects::SmtEffectsCount];
    for(int i = 0; i < ExpPlayerEffects::SmtEffectsCount; ++i) {
        effects.rk_orangeMT[i] = new(EGG::Effect)(ExpPlayerEffects::SMTNames[i], effects.playerIdPlus2);
    }
};
kmCall(0x8068e9c4, LoadCustomEffects);

//Left and Right sparks when the SMT charge is over 550
void LoadLeftPurpleSparkEffects(ExpPlayerEffects& effects, EGG::Effect** effectArray, u32 firstEffectIndex, u32 lastEffectIndex, const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale) {
    int type = effects.kartObject->getDriftType();
    u32 umtCharge = effects.kartObject->pointers.kartMove->umtCharge;
    if (umtCharge >= 250 && type == KartStats::KART) {
        effects.createAndUpdateEffectsByIdx(effects.rk_purpleMT, 0, 2, playerMat2, wheelPos, updateScale);
        effects.followFadeEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
    }
    else effects.createAndUpdateEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
};
kmCall(0x80698a94, LoadLeftPurpleSparkEffects);

void LoadRightPurpleSparkEffects(ExpPlayerEffects& effects, EGG::Effect** effectArray, u32 firstEffectIndex, u32 lastEffectIndex, const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale) {
    int type = effects.kartObject->getDriftType();
    u32 umtCharge = effects.kartObject->pointers.kartMove->umtCharge;
    if (umtCharge >= 250 && type == KartStats::KART) {
        effects.createAndUpdateEffectsByIdx(effects.rk_purpleMT, 2, 4, playerMat2, wheelPos, updateScale);
        effects.followFadeEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
    }
    else effects.createAndUpdateEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
};
kmCall(0x80698af0, LoadRightPurpleSparkEffects);

void LoadOrangeSparkEffects(ExpPlayerEffects& effects, EGG::Effect** effectArray, u32 firstEffectIndex, u32 lastEffectIndex, const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale) {
    int type = effects.kartObject->getDriftType();
    u32 smtCharge = effects.kartObject->pointers.kartMove->smtCharge;
    if (smtCharge >= 300 && type == KartStats::OUTSIDE_BIKE) {
        effects.createAndUpdateEffectsByIdx(effects.rk_orangeMT, 0, 2, playerMat2, wheelPos, updateScale);
        effects.followFadeEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
    }
    else effects.createAndUpdateEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
};
kmBranch(0x806a2f60, LoadOrangeSparkEffects);
kmBranch(0x806a3004, LoadOrangeSparkEffects);

//Fade the sparks
void FadeLeftPurpleSparkEffects(ExpPlayerEffects& effects, EGG::Effect** effectArray, u32 firstEffectIndex, u32 lastEffectIndex, const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale) {
    effects.followFadeEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
    effects.followFadeEffectsByIdx(effects.rk_purpleMT, 0, 2, playerMat2, wheelPos, updateScale);
};
kmCall(0x80698dac, FadeLeftPurpleSparkEffects);
kmCall(0x80698228, FadeLeftPurpleSparkEffects);
kmCall(0x80698664, FadeLeftPurpleSparkEffects);
kmCall(0x80698ab4, FadeLeftPurpleSparkEffects);

void FadeRightPurpleSparkEffects(ExpPlayerEffects& effects, EGG::Effect** effectArray, u32 firstEffectIndex, u32 lastEffectIndex, const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale) {
    effects.followFadeEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
    effects.followFadeEffectsByIdx(effects.rk_purpleMT, 2, 4, playerMat2, wheelPos, updateScale);
};
kmCall(0x80698248, FadeRightPurpleSparkEffects);
kmCall(0x80698684, FadeRightPurpleSparkEffects);
kmCall(0x80698b10, FadeRightPurpleSparkEffects);
kmCall(0x80698dcc, FadeRightPurpleSparkEffects);

void FadeOrangeSparkEffects(ExpPlayerEffects& effects, EGG::Effect** effectArray, u32 firstEffectIndex, u32 lastEffectIndex, const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale) {
    effects.followFadeEffectsByIdx(effectArray, firstEffectIndex, lastEffectIndex, playerMat2, wheelPos, updateScale);
    effects.followFadeEffectsByIdx(effects.rk_orangeMT, 0, 2, playerMat2, wheelPos, updateScale);
};
kmBranch(0x806a31fc, FadeOrangeSparkEffects);

int PatchDriftStateCheck(const KartObject& kartObject) {
    u32 driftState = kartObject.getDriftState();
    if (driftState == 4) driftState = 3;
    return driftState;
};
kmCall(0x8069807c, PatchDriftStateCheck);

// Purple boosts
kmWrite32(0x806a3d00, 0x7FA4EB78);
kmWrite32(0x806a3d04, 0x7FC5F378);
void PatchBoostOnUMTSpeedBoost(EGG::Effect* boostEffect) { //have to mod loop index by 4 to get the actual index
    register u8 loopIndex;
    asm(mr loopIndex, r29;);
    register ExpPlayerEffects* effects;
    asm(mr effects, r30;);

    KartMove* movement = effects->kartObject->pointers.kartMove;
    if (umtState[effects->playerId]) boostEffect = effects->rk_purpleMT[ExpPlayerEffects::rk_purpleBoost + loopIndex % 4];
    boostEffect->create();
};
kmCall(0x806a3d08, PatchBoostOnUMTSpeedBoost);

kmWrite32(0x8069bfa0, 0x60000000);
void PatchBoostMatrix(EGG::Effect* boostEffect, MTX34* boostMat) {
    if (boostEffect->effectHandle.GetPtr()) {
        boostEffect->setMtx(boostMat);
        boostEffect->update();
    }
    register u8 loopIndex;
    asm(mr loopIndex, r29;);
    register ExpPlayerEffects* effects;
    asm(mr effects, r30;);
    if (!effects->isBike) {
        boostEffect = effects->rk_purpleMT[ExpPlayerEffects::rk_purpleBoost + loopIndex % 4];
        if (boostEffect->effectHandle.GetPtr()) {
            boostEffect->setMtx(boostMat);
            boostEffect->update();
        }
    }
}
kmCall(0x8069bff0, PatchBoostMatrix);
kmWrite32(0x8069c004, 0x60000000);


void PatchFadeBoost(EGG::Effect* boostEffect) {
    boostEffect->followFade();
    register u8 loopIndex;
    asm(mr loopIndex, r29;);
    register ExpPlayerEffects* effects;
    asm(mr effects, r30;);
    if (!effects->isBike) effects->rk_purpleMT[ExpPlayerEffects::rk_purpleBoost + loopIndex % 4]->followFade();
}
kmCall(0x8069c0a4, PatchFadeBoost);

//nw4r::snd::SoundStartable::StartResult PlayExtBRSEQ(nw4r::snd::SoundStartable& startable, void* handle, const char* fileName, const char* labelName, bool hold) {
//    nw4r::snd::SoundStartable::StartInfo startInfo;
//    startInfo.seqSoundInfo.startLocationLabel = labelName;
//    startInfo.enableFlag |= nw4r::snd::SoundStartable::StartInfo::ENABLE_SEQ_SOUND_INFO;
//
//    void* file = ResourceManager::instance->getFile(MultiDvdArchive::COMMON, fileName);
//    if (file != nullptr) {
//        startInfo.seqSoundInfo.seqDataAddress = file;
//        if (hold) return startable.detail_HoldSound(&handle, 0x19a, &startInfo); //SOUND_ID_MINITURBO
//        else return startable.detail_StartSound(&handle, 0x19a, &startInfo);
//    }
//    return nw4r::snd::SoundStartable::START_ERR_USER;
//}

//kmWrite32(0x807095b8, 0x40A00028); //changes beq to bge for UMT
//void PatchUMTSound(AudioHandleHolder& sound, u32 soundId, void* handle) {
//    if (sound.driftState == 4) {
//        const char* seqName = "purpleMT.brseq";
//        const char* labelName = "b";
//        nw4r::snd::SoundStartable::StartResult ret = PlayExtBRSEQ(reinterpret_cast<nw4r::snd::SoundStartable&>(sound), handle, seqName, labelName, true);
//        if (ret == snd::SoundStartable::START_SUCCESS) return;
//    }
//    sound.instance->prepare(soundId, &handle);
//
//};
//kmCall(0x807095f8, PatchUMTSound);
