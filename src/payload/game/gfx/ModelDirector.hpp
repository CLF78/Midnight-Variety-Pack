#pragma once
#include "ModelTransformator.hpp"
#include <game/system/MultiDvdArchive.hpp>
#include <game/system/ResourceManager.hpp>

class ModelDirector {
public:
    static void bindBRRES(void* file, MultiDvdArchive::Type type, const char* brresName);
    static void bindBRRESFallback(void* file, MultiDvdArchive::Type type, const char* brresName, const void* fallbackFile);
    static void bindBRRESImpl(void* file, const char* name, const void* fallbackFile, u32 unused);

    UNK(0x28);
    ModelTransformator* modelTransformator;
    UNK(0x4C - 0x2C);
};
size_assert(ModelDirector, 0x4C);
