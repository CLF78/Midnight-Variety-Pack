#pragma once
#include <egg/core/eggExpHeap.hpp>
#include <egg/core/eggScene.hpp>

struct HeapCollection {

    enum HeapTag {
        DEFAULT,
        RACE_DATA,
        MISC,
        UNK_3,
        GEO_OBJ,
        JMAP_AND_MDL,
        MENU,
        EFFECT,
        AUDIO,
        UNK_9,
        RES,
        HBM,
        UNK_C,
        NETWORKING,
    };

    EGG::ExpHeap* heapMem1;
    EGG::ExpHeap* heapMem2;
    EGG::ExpHeap* heapMemDebug;
    EGG::ExpHeap::GroupSizeRecord records[3];
};
size_assert(HeapCollection, 0xC0C);

class Scene : public EGG::Scene {
public:
    enum SceneId {
        SCENE_ROOT,
        SCENE_MENU,
        SCENE_RACE,
        SCENE_DEBUG_3,
        SCENE_GLOBE,
        SCENE_BOOTSTRAP,
        SCENE_DEBUG_6,
        SCENE_DEBUG_7,
        SCENE_DEBUG_8,
        SCENE_DEBUG_9,
        SCENE_DEBUG_A,
        SCENE_DEBUG_B,
        SCENE_FLAG_OPEN,
    };

    HeapCollection heapCollection;
    u8 _C3C[0xC70 - 0xC3C]; // unused
};
size_assert(Scene, 0xC70);
