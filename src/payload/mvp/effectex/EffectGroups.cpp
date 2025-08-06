#include "EffectGroups.hpp"
#include <egg/effect/eggEffect.hpp>
#include <game/effect/PlayerEffects.hpp>

void KartUmtEffects::Create(PlayerEffects* playerEffects) {

    // Get creator idx
    int creatorIdx = playerEffects->playerIdPlus2;

    // Create looped sparks
    if (playerEffects->isHumanPlayer || playerEffects->isGhost) {
        CREATE_EFFECT(rk_driftSpark3L_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3L_Spark01, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3R_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3R_Spark01, creatorIdx);
    }
    else if (playerEffects->isHumanPlayer2P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3L_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3L_Spark01, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3R_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3R_Spark01, "M", creatorIdx);
    }
    else if (playerEffects->isHumanPlayer3P4P || playerEffects->isRemoteCPU || playerEffects->isRemoteCPU2P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3L_Spark01, "S", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3R_Spark01, "S", creatorIdx);
    }

    // Create single fire sparks
    if (playerEffects->isHumanPlayer) {
        CREATE_EFFECT(rk_driftSpark3L1T_Chip00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3L1T_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3L1T_Spark01, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3R1T_Chip00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3R1T_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3R1T_Spark01, creatorIdx);
    }
    else if (playerEffects->isHumanPlayer2P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3L1T_Chip00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3L1T_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3L1T_Spark01, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3R1T_Chip00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3R1T_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3R1T_Spark01, "M", creatorIdx);
    }

    // Create charging chips
    if (playerEffects->isHumanPlayer) {
        CREATE_EFFECT(rk_driftSpark3L_Chip00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark3R_Chip00, creatorIdx);
    }
    else if (playerEffects->isHumanPlayer2P || playerEffects->isHumanPlayer3P4P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3L_Chip00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark3R_Chip00, "M", creatorIdx);
    }
}

void BikeSmtEffects::Create(PlayerEffects* playerEffects) {

    // Get creator idx
    int creatorIdx = playerEffects->playerIdPlus2;

    // Create looped sparks
    if (playerEffects->isHumanPlayer || playerEffects->isGhost) {
        CREATE_EFFECT(rk_driftSpark2LB_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2LB_Spark01, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2RB_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2RB_Spark01, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2CB_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2CB_Spark01, creatorIdx);
    }
    else if (playerEffects->isHumanPlayer2P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2LB_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2LB_Spark01, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2RB_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2RB_Spark01, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2CB_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2CB_Spark01, "M", creatorIdx);
    }
    else if (playerEffects->isHumanPlayer3P4P || playerEffects->isRemoteCPU || playerEffects->isRemoteCPU2P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2LB_Spark01, "S", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2RB_Spark01, "S", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2CB_Spark01, "S", creatorIdx);
    }

    // Create single fire sparks
    if (playerEffects->isHumanPlayer) {
        CREATE_EFFECT(rk_driftSpark2LB1T_Chip00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2LB1T_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2LB1T_Spark01, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2RB1T_Chip00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2RB1T_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2RB1T_Spark01, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2CB1T_Chip00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2CB1T_Spark00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2CB1T_Spark01, creatorIdx);
    }
    else if (playerEffects->isHumanPlayer2P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2LB1T_Chip00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2LB1T_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2LB1T_Spark01, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2RB1T_Chip00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2RB1T_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2RB1T_Spark01, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2CB1T_Chip00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2CB1T_Spark00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2CB1T_Spark01, "M", creatorIdx);
    }

    // Create charging chips
    if (playerEffects->isHumanPlayer) {
        CREATE_EFFECT(rk_driftSpark2LB_Chip00, creatorIdx);
        CREATE_EFFECT(rk_driftSpark2RB_Chip00, creatorIdx);
    }
    else if (playerEffects->isHumanPlayer2P || playerEffects->isHumanPlayer3P4P) {
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2LB_Chip00, "M", creatorIdx);
        CREATE_EFFECT_WITH_SUFFIX(rk_driftSpark2RB_Chip00, "M", creatorIdx);
    }
}
