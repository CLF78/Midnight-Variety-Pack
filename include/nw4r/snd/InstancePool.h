namespace nw4r {
namespace snd {
namespace detail {

class PoolImpl {
    public:
        PoolImpl* next;
};
size_assert(PoolImpl, 0x4);

} // namespace detail
} // namespace snd
} // namespace nw4r
