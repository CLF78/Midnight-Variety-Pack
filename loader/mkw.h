#include <kamek.h>
#include <egg/core/eggExpHeap.h>
#include <egg/core/eggScene.h>
#include <egg/core/eggSceneManager.h>
#include "kamekLoader.h"

#define REGION_P 0
#define REGION_E 1
#define REGION_J 2
#define REGION_K 3

typedef void* (*EGG_Heap_Alloc_t) (u32 size, s32 align, EGG::Heap* heap);
typedef void (*EGG_Heap_Free_t) (void* buffer, EGG::Heap* heap);
typedef EGG::ExpHeap* (*EGG_ExpHeap_Create_t) (void* block, u32 size, u16 options);

struct loaderFunctionsEx {
    loaderFunctions base;
    EGG_Heap_Alloc_t eggAlloc;
    EGG_Heap_Free_t eggFree;
    EGG_ExpHeap_Create_t expHeapCreate;
    EGG::SceneManager** sceneMgr;
    u8 identifier;
};
