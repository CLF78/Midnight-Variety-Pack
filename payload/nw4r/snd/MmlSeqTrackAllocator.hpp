#include <nw4r/snd/InstancePool.hpp>
#include <nw4r/snd/MmlParser.hpp>
#include <nw4r/snd/SeqTrackAllocator.hpp>

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
