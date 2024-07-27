#include <common/Common.hpp>
#include <game/kart/KartObjectProxy.hpp>

enum KCLTypes {
    KCL_ROAD,
    KCL_SLIPPERY_ROAD,
    KCL_WEAK_OFFROAD,
    KCL_NORMAL_OFFROAD,
    KCL_HEAVY_OFFROAD,
    KCL_ICY_ROAD,
    KCL_BOOST_PANEL,
    KCL_BOOST_RAMP,
    KCL_JUMP_PAD,
    KCL_ITEM_ROAD,
    KCL_SOLID_FALL,
    KCL_MOVING_WATER,
    KCL_WALL,
    KCL_INVISIBLE_WALL,
    KCL_ITEM_WALL,
    KCL_WALL_3,
    KCL_FALL_BOUNDARY,
    KCL_CANNON_TRIGGER,
    KCL_FORCE_RECALCULATION,
    KCL_HALFPIPE,
    KCL_PLAYER_ONLY_WALL,
    KCL_MOVING_ROAD,
    KCL_STICKY_ROAD,
    KCL_ROAD2,
    KCL_SOUND_TRIGGER,
    KCL_WEAK_WALL,
    KCL_EFFECT_TRIGGER,
    KCL_ITEM_STATE,
    KCL_HALFPIPE_INVIS_WALL,
    KCL_ROULETTE_MOVING_ROAD,
    KCL_SPECIAL_WALL,
    KCL_WALL5                   
};

enum KCLBitfield {
    KCL_BITFIELD_ROAD                   = 1 << KCL_ROAD,                
    KCL_BITFIELD_SLIPPERY_ROAD          = 1 << KCL_SLIPPERY_ROAD,       
    KCL_BITFIELD_WEAK_OFFROAD           = 1 << KCL_WEAK_OFFROAD,        
    KCL_BITFIELD_NORMAL_OFFROAD         = 1 << KCL_NORMAL_OFFROAD,      
    KCL_BITFIELD_HEAVY_OFFROAD          = 1 << KCL_HEAVY_OFFROAD,       
    KCL_BITFIELD_ICY_ROAD               = 1 << KCL_ICY_ROAD,            
    KCL_BITFIELD_BOOST_PANEL            = 1 << KCL_BOOST_PANEL,         
    KCL_BITFIELD_BOOST_RAMP             = 1 << KCL_BOOST_RAMP,          
    KCL_BITFIELD_JUMP_PAD               = 1 << KCL_JUMP_PAD,            
    KCL_BITFIELD_ITEM_ROAD              = 1 << KCL_ITEM_ROAD,           
    KCL_BITFIELD_SOLID_FALL             = 1 << KCL_SOLID_FALL,          
    KCL_BITFIELD_MOVING_WATER           = 1 << KCL_MOVING_WATER,        
    KCL_BITFIELD_WALL                   = 1 << KCL_WALL,                
    KCL_BITFIELD_INVISIBLE_WALL         = 1 << KCL_INVISIBLE_WALL,      
    KCL_BITFIELD_ITEM_WALL              = 1 << KCL_ITEM_WALL,           
    KCL_BITFIELD_WALL_3                 = 1 << KCL_WALL_3,              
    KCL_BITFIELD_FALL_BOUNDARY          = 1 << KCL_FALL_BOUNDARY,       
    KCL_BITFIELD_CANNON_TRIGGER         = 1 << KCL_CANNON_TRIGGER,      
    KCL_BITFIELD_FORCE_RECALCULATION    = 1 << KCL_FORCE_RECALCULATION, 
    KCL_BITFIELD_HALFPIPE               = 1 << KCL_HALFPIPE,            
    KCL_BITFIELD_PLAYER_ONLY_WALL       = 1 << KCL_PLAYER_ONLY_WALL,    
    KCL_BITFIELD_MOVING_ROAD            = 1 << KCL_MOVING_ROAD,         
    KCL_BITFIELD_STICKY_ROAD            = 1 << KCL_STICKY_ROAD,         
    KCL_BITFIELD_ROAD2                  = 1 << KCL_ROAD2,               
    KCL_BITFIELD_SOUND_TRIGGER          = 1 << KCL_SOUND_TRIGGER,       
    KCL_BITFIELD_WEAK_WALL              = 1 << KCL_WEAK_WALL,           
    KCL_BITFIELD_EFFECT_TRIGGER         = 1 << KCL_EFFECT_TRIGGER,      
    KCL_BITFIELD_ITEM_STATE             = 1 << KCL_ITEM_STATE,          
    KCL_BITFIELD_HALFPIPE_INVIS_WALL    = 1 << KCL_HALFPIPE_INVIS_WALL, 
    KCL_BITFIELD_ROULETTE_MOVING_ROAD   = 1 << KCL_ROULETTE_MOVING_ROAD,
    KCL_BITFIELD_SPECIAL_WALL           = 1 << KCL_SPECIAL_WALL,        
    KCL_BITFIELD_WALL5                  = 1 << KCL_WALL5                
};

class KartCollideBase {
public:
    virtual void processVehicleBodyCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags) = 0;
    virtual void processWheelCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags) = 0;
    virtual ~KartCollideBase() = 0;
};

class KartCollide : public KartCollideBase, KartObjectProxy {
public:
    virtual void processVehicleBodyCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags);
    virtual void processWheelCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags);
    virtual ~KartCollide();

    void activateOob(bool unk, u32* kclFlags, bool unk2, bool unk3);

    u8 unk[0x6C-0x10];
    s16 respawnArea;
    u8 unk2[0x74-0x6E];
};
size_assert(KartCollide, 0x74);
