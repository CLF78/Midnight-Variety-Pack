#pragma once
#include "InstancePool.hpp"
#include "MmlParser.hpp"
#include "SeqTrackAllocator.hpp"

namespace nw4r {
namespace snd {
namespace detail {

class MmlSeqTrackAllocator : public SeqTrackAllocator {
public:
    MmlParser* parser;
    PoolImpl trackPool;
};
size_assert(MmlSeqTrackAllocator, 0xC);

} // namespace detail
} // namespace snd
} // namespace nw4r
