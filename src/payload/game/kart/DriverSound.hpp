class DriverSound {
public:

    enum SoundType {
        USE_ITEM          = 0x0,
        THROW_ITEM        = 0x1,
        USE_STAR_MEGA     = 0x2,
        HIT_ITEM          = 0x3,
        BOOST             = 0x6,
        NO_STUNT_TRICK    = 0x8,
        NO_FLIP_TRICK     = 0xA,
        STUNT_1_FLIP      = 0xB,
        STUNT_2_FLIPS     = 0xC,
        NO_FLIP_LANDING   = 0xD,
        WHEELIE           = 0xF,
        WHEELIE_LOW_SPEED = 0x10,
        HIT_WALL          = 0x11,
        HITBY_OBJECT      = 0x12,
        HITBY_EXPLOSION   = 0x13,
        HITBY_FIRE        = 0x14,
        HITBY_BANANA      = 0x15,
        RESPAWN           = 0x1A,
        DRAFT             = 0x1C,
    };
};
