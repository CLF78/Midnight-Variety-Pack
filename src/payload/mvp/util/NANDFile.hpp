#pragma once
#include <common/Common.hpp>
#include <egg/core/eggHeap.hpp>
#include <game/util/NandUtil.hpp>

class NANDFileHandler {
public:

    NANDFileHandler(const char* path, int access) : mResult(NandUtil::Open(path, &mInfo, access)) {}
    ~NANDFileHandler() { if (mResult == NandUtil::ERROR_NONE) NandUtil::Close(&mInfo); }

    NANDFileInfo mInfo;
    int mResult;
};

class NANDFileBuffer {
public:
    NANDFileBuffer(u32 size, EGG::Heap* heap) { mBuffer = new (heap, 32) u8[size]; }
    ~NANDFileBuffer() { if (mBuffer) delete mBuffer; }

    u8* mBuffer;
};
