#include <kamek.h>
#include <egg/core/eggExpHeap.h>

class RKSystem {
public:
    virtual void* getVideo();
    virtual EGG::Heap* getSysHeap();
    virtual void* getDisplay();
    virtual void* getXFBManager();
    virtual void* getProcessMeter();
    virtual void* getSceneManager();
    virtual void* getAudioManager();
    virtual void onBeginFrame();
    virtual void onEndFrame();
    virtual void initRenderMode();
    virtual void initMemory();
    virtual void run();
    virtual void initSystem();

    void* MEM1ArenaLo;
    void* MEM1ArenaHi;
    void* MEM2ArenaLo;
    void* MEM2ArenaHi;
    u32 memorySize;
    EGG::ExpHeap* EGGRootMEM1;
    EGG::ExpHeap* EGGRootMEM2;
    EGG::ExpHeap* EGGRootDebug;
    EGG::ExpHeap* EGGSystem;
    void* heapSystemThread;
    u32 _2C;
    u32 _30;
    u32 sysHeapSize;
    u32 gxFifoBufSize;
    void* renderMode;
    void* audioManager;
    void* video;
    void* xfbManager;
    void* asyncDisplay;
    void* processMeter;
    void* sceneManager;
    EGG::ExpHeap* kpadWorkHeap;
    u32 _5C;
    EGG::ExpHeap* relLinkHeap;
    EGG::ExpHeap* heapSystem2;
    u8 frameClock;
    u8 _69;
    bool drawFrame;
    u8 _6B;
    u8 _6C;
    u32 _70;
};
size_assert(RKSystem, 0x74);
