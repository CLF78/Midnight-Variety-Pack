#pragma once

class MessageGroup {
public:
    struct Header {
        u32 signature;
        u32 dataType;
        u32 dataSize;
        u32 numBlocks;
        u8 charset;
        PAD(0xF);
    };
    size_assert(Header, 0x20);

    struct Block {
        u32 kind;
        u32 size;
        u8 data[];
    };
    size_assert(Block, 0x8);

    enum BlockKind {
        BLOCK_KIND_INF = 'INF1',
        BLOCK_KIND_DAT = 'DAT1',
        BLOCK_KIND_STR = 'STR1',
        BLOCK_KIND_MID = 'MID1',
    };

    struct Entry {
        u32 offset;
        u8 attributes;
        PAD(3);
    };
    size_assert(Entry, 0x8);

    struct Inf {
        u16 numEntries;
        u16 entrySize;
        u16 groupID;
        u8 defaultColor;
        PAD(1);
        Entry entries[];
    };
    size_assert(Inf, 0x8);

    struct Mid {
        u16 numEntries;
        u8 format;
        u8 info;
        PAD(4);
        u32 messageIds[];
    };
    size_assert(Mid, 0x8);

    const Header* header;
    const Inf* inf;
    const wchar_t* dat;
    const void* str;
    const Mid* mid;
};
size_assert(MessageGroup, 0x14);
