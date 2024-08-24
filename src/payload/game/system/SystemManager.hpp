#pragma once
#include <egg/core/eggHeap.hpp>
#include <egg/core/eggTaskThread.hpp>
#include <revolution/os/OSMessage.h>
#include <revolution/os/OSModule.h>
#include <revolution/sc.h>

class SystemManager {
public:
    class SystemRipper {
        char pathBuf[64];
        EGG::Heap* heap;
        u8 allocDirection;
        PAD(3);

        u32* fileSize;
        u8* manualDestBuffer;
        u8* destBuffer;
    };
    size_assert(SystemRipper, 0x54);

    enum Region {
        REGION_JAPAN,
        REGION_AMERICA,
        REGION_EUROPE,
        REGION_OCEANIA,
        REGION_TAIWAN,
        REGION_KOREA,
        REGION_CHINA,
        REGION_COUNT,
        REGION_NONE = 15,
    };

    SystemRipper systemRipper;
    virtual ~SystemManager();

    u32 aspectRatio;

    u32 language;
    u32 strapLanguage;

    u32 progressiveMode;
    u32 isRgb60;
    u32 isEuro;
    u32 launchType;

    float framerate;
    float frameratePAL50;
    float frameDuration;

    bool isNdev;
    PAD(1);
    u16 launchCode;

    u32 regionId;
    u8* _88; // unused
    u8* _8C; // unused
    u8* _90; // unused

    u8* symbolMap;
    char* ndevArg;

    u32 dolStart;
    u32 dolEnd;
    u32 dolSize;
    u32 _A8; // unused

    EGG::Heap* heap;
    OSModuleInfo* moduleInfo;

    bool doShutDown;
    bool doRestart;
    bool _B6;
    bool obeyShutdownRequests;

    OSMessageQueue msgQueue;
    void* messages[4];

    EGG::TaskThread* taskThread;
    OSThread* someThread;

    u8 year;
    u8 month;
    u8 dayOfMonth;

    bool _F3;
    u8 _F4; // unused
    PAD(3);

    SCSimpleAddressData addressData;

    static SystemManager* instance;

    static void Restart();
    static void ReturnToMenu();
    static void ShutdownSystem();
};
size_assert(SystemManager, 0x1100);
