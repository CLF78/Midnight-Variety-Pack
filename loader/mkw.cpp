#include <kamek.h>
#include "kamekLoader.h"
#include "mkw.h"

// Defines
#define directWriteBranch(addr, dest, lk) *(addr)=((((u32)(dest)-(u32)(addr)) & 0x3FFFFFC) | 0x48000000 | (lk))
#define CUSTOM_HEAP_END 0x809ABD20

// Static region-free addresses
extern "C" {
u16 detect:0x8000620A;
u32 relHook:0x800074D4;
EGG::ExpHeap codeHeap:0x808DD400;
vu32 aiControl:0xCD006C00;
}

// Game-specific functions
EGG::ExpHeap* getRootSceneHeap(const loaderFunctionsEx* funcsEx) {
    return (EGG::ExpHeap*)(*(funcsEx->sceneMgr))->currentScene->heapMem2;
}

void* allocAdapter(u32 size, bool isForCode, const loaderFunctions* funcs) {
    const loaderFunctionsEx* funcsEx = (const loaderFunctionsEx* )funcs;
    EGG::ExpHeap* heapPtr = isForCode ? &codeHeap : getRootSceneHeap(funcsEx);
    return funcsEx->eggAlloc(size, 0x20, heapPtr);
}

void freeAdapter(void* buffer, bool isForCode, const loaderFunctions* funcs) {
    const loaderFunctionsEx* funcsEx = (const loaderFunctionsEx* )funcs;
    EGG::ExpHeap* heapPtr = isForCode ? &codeHeap : getRootSceneHeap(funcsEx);
    funcsEx->eggFree(buffer, heapPtr);
}

// Loader data
const loaderFunctionsEx functions[4] = {
    {{(OSReport_t) 0x801A25D0,
    (OSFatal_t) 0x801A4EC4,
    (DVDConvertPathToEntrynum_t) 0x8015DF4C,
    (DVDFastOpen_t) 0x8015E254,
    (DVDReadPrio_t) 0x8015E834,
    (DVDClose_t) 0x8015E568,
    (sprintf_t) 0x80011A2C,
    allocAdapter,
    freeAdapter},
    (EGG_Heap_Alloc_t) 0x80229814,
    (EGG_Heap_Free_t) 0x80229B84,
    (EGG_ExpHeap_Create_t) 0x80226A1C,
    (EGG::SceneManager**) 0x802A40D4,
    'P'},

    {{(OSReport_t) 0x801A2530,
    (OSFatal_t) 0x801A4E24,
    (DVDConvertPathToEntrynum_t) 0x8015DEAC,
    (DVDFastOpen_t) 0x8015E1B4,
    (DVDReadPrio_t) 0x8015E794,
    (DVDClose_t) 0x8015E4C8,
    (sprintf_t) 0x80010ECC,
    allocAdapter,
    freeAdapter},
    (EGG_Heap_Alloc_t) 0x80229490,
    (EGG_Heap_Free_t) 0x80229800,
    (EGG_ExpHeap_Create_t) 0x80226698,
    (EGG::SceneManager**) 0x8029FD54,
    'E'},

    {{(OSReport_t) 0x801A24F0,
    (OSFatal_t) 0x801A4DE4,
    (DVDConvertPathToEntrynum_t) 0x8015DE6C,
    (DVDFastOpen_t) 0x8015E174,
    (DVDReadPrio_t) 0x8015E754,
    (DVDClose_t) 0x8015E488,
    (sprintf_t) 0x80011950,
    allocAdapter,
    freeAdapter},
    (EGG_Heap_Alloc_t) 0x80229734,
    (EGG_Heap_Free_t) 0x80229AA4,
    (EGG_ExpHeap_Create_t) 0x8022693C,
    (EGG::SceneManager**) 0x802A3A54,
    'J'},

    {{(OSReport_t) 0x801A292C,
    (OSFatal_t) 0x801A5220,
    (DVDConvertPathToEntrynum_t) 0x8015DFC4,
    (DVDFastOpen_t) 0x8015E2CC,
    (DVDReadPrio_t) 0x8015E8AC,
    (DVDClose_t) 0x8015E5E0,
    (sprintf_t) 0x80011A94,
    allocAdapter,
    freeAdapter},
    (EGG_Heap_Alloc_t) 0x80229B88,
    (EGG_Heap_Free_t) 0x80229EF8,
    (EGG_ExpHeap_Create_t) 0x80226D90,
    (EGG::SceneManager**) 0x802920D4,
    'K'}
};

void unknownVersion() {
    // can't do much here!
    // we can't output a message on screen without a valid OSFatal addr
    for (;;);
}

// Region detection function
u32 regionDetect() {

    // Default to PAL
    u32 region = REGION_P;

    // Use instruction to detect region
    switch (detect) {
        case 0x54A9: break;
        case 0x5409: region = REGION_E; break;
        case 0x53CD: region = REGION_J; break;
        case 0x5511: region = REGION_K; break;
        default: unknownVersion();
    }

    return region;
}

// Second stage payload loader - loads Kamek binary
u32 secondStage(u32 ret) {

    // Detect region and choose functions
    u32 region = regionDetect();
    const loaderFunctionsEx* funcs = &functions[region];

    // Create custom heap
    funcs->expHeapCreate(&codeHeap, CUSTOM_HEAP_END - (u32)&codeHeap, 0);

    // Load file
    char path[] = "/codeX.bin";
    path[5] = funcs->identifier;
    loadKamekBinaryFromDisc(&funcs->base, path);
    return ret;
}

// First stage payload loader - applies audio fix and inserts hook to second stage
void firstStage() {

    // Before we can do anything, detect region
    u32 region = regionDetect();
    const loaderFunctionsEx* funcs = &functions[region];

    // Reset the DSP to fix libogc clownery;
    aiControl = 0;

    // Insert second stage hook
    directWriteBranch(&relHook, &secondStage, false);
    cacheInvalidateAddress(&relHook);
}

// First stage injection - this is the earliest safe spot we can act at
kmCall(0x800060BC, firstStage);
