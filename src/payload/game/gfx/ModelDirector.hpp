#pragma once
#include <common/Common.hpp>
#include <game/gfx/ModelTransformator.hpp>
#include <game/system/ResourceManager.hpp>
#include <game/system/MultiDvdArchive.hpp>

class ModelDirector {
public:    
    static void bindBRRES(void* file, MultiDvdArchive::Type type, const char* brresName);
    static void bindBRRESFallback(void* file, MultiDvdArchive::Type type, const char* brresName, const void* fallbackFile);
    static void bindBRRESImpl(void* file, const char* name, const void* fallbackFile, u32 unused);
    u8 unk[0x28];
    ModelTransformator* modelTransformator;
    u8 unk2[0x4C-0x2C];
};
size_assert(ModelDirector, 0x4C);
