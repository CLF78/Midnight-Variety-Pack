#include <kamek.h>
#include <egg/core/eggDisposer.h>
#include <stdlib/new.h>
#include "saveexpansion/RawSaveExpansion.h"
#include "saveexpansion/SaveExpansion.h"

class SaveManagerEx {
public:
    SaveManagerEx() {}

    static SaveManagerEx* construct(void* buffer) {
        return new(buffer) SaveManagerEx;
    }

    RawSaveExpansion::Header* rawExpansion;
    SaveExpansion licensesEx[4];
};

class SaveManager : public EGG::Disposer {
public:
    virtual ~SaveManager();

    void* rawSave;
    void* rawGhostFile;
    void* taskThread;

    u32 ghostOffset;
    bool _24;

    void* ghostGroup;
    s32 _2C;
    s32 _30;

    u8 bannerType;
    bool saveGhostResult;

    s16 currentLicenseId;
    u8 licenses[0x24FC0];

    bool initialized;
    void* rawSaveCopy;

    bool busy;
    bool valid;
    bool canSave;

    s32 result;
    SaveManagerEx expansion;

    static SaveManager* instance;
};
size_assert(SaveManager, 0x25008 + sizeof(SaveManagerEx));
