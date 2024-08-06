#pragma once
#include <revolution/mtx.h>

struct KmpSectionHeader {
    s32 sectionMagic;
    u16 entryCount;
    s8 extraValue[2];
};
size_assert(KmpSectionHeader, 0x8);

template <typename T>
class MapdataAccessorBase {
public:
    T** entries;
    u16 numEntries;
    PAD(2);
    KmpSectionHeader* sectionHeader;
};

class MapdataPointInfo {
public:
    struct Point {
        VEC3 pos;
        u16 settings[2];
    };
    size_assert(Point, 0x10);

    struct SData {
        u16 pointCount;
        u8 settings[2];
        Point points[];
    };
    size_assert(SData, 0x4);

    SData* mpData;
};
size_assert(MapdataPointInfo, 0x4);

class MapdataCamera {
public:
    struct SData {
        u8 cameraType;
        u8 cameraNext;
        u8 cameraShake;
        u8 pathID;
        u16 pathSpeed;
        u16 fovYSpeed;
        u16 viewSpeed;
        u8 startFlag;
        u8 movieFlag;
        VEC3 position;
        VEC3 rotation;
        f32 fovYStart;
        f32 fovVYEnd;

        VEC3 viewStart;
        VEC3 viewEnd;

        f32 time;
    };
    size_assert(SData, 0x48);

    SData* mpData;
};
size_assert(MapdataCamera, 0x4);

class MapdataCannonPoint {
public:
    struct SData {
        VEC3 position;
        VEC3 rotation;
        u16 id;
        u16 shootEffect;
    };
    size_assert(SData, 0x1C);

    SData* mpData;
};
size_assert(MapdataCannonPoint, 0x4);

class MapdataCheckPath {
public:
    struct SData {
        u8 start;
        u8 size;

        u8 last[6];
        u8 next[6];
        PAD(2);
    };
    size_assert(SData, 0x10);

    SData* mpData;
    u8 dfsDepth;
    PAD(3);
    float _8;
};
size_assert(MapdataCheckPath, 0xC);

class MapdataCheckPoint {
public:
    struct SData {
        VEC2 left;
        VEC2 right;
        u8 jugemIndex;
        u8 lapCheck;
        u8 prevPt;
        u8 nextPt;
    };
    size_assert(SData, 0x14);

    struct Link {
        MapdataCheckPoint* checkpoint;
        VEC2 p0diff;
        VEC2 p1diff;
        float distance;
    };
    size_assert(Link, 0x18);

    SData* mpData;

    u16 nextCount;
    u16 prevCount;

    VEC2 midPoint;
    VEC2 dir;

    u16 _18;
    u16 id;

    u8 prevKcpId;
    PAD(3);
    MapdataCheckPoint* prevCheckPoints[6];
    Link nextCheckPoints[6];
};
size_assert(MapdataCheckPoint, 0xC8);

class MapdataEnemyPath {
public:
    struct SData {
        u8 start;
        u8 size;
        u8 last[6];
        u8 next[6];
        u8 battle_params[2];
    };
    size_assert(SData, 0x10);

    SData* mpData;
    UNK(0x0C - 0x04);
};
size_assert(MapdataEnemyPath, 0xC);

class MapdataEnemyPoint {
public:
    struct SData {
        VEC3 position;
        f32 deviation;
        u8 parameters[4];
    };
    size_assert(SData, 0x14);

    virtual ~MapdataEnemyPoint();
    SData* mpData;

    u32* _8;
    u32* _c;
    u32 _10;
    u8 _14;
    PAD(3);
};
size_assert(MapdataEnemyPoint, 0x18);

class MapdataGeoObj {
public:
    struct SData {
        u16 id;
        PAD(2);
        VEC3 translation;
        VEC3 rotation;
        VEC3 scale;
        s16 pathId;
        u16 settings[8];
        u16 presenceFlag;
    };
    size_assert(SData, 0x3C);

    SData* mpData;
};
size_assert(MapdataGeoObj, 0x4);

class MapdataItemPath {
public:
    struct SData {
        u8 start;
        u8 size;

        u8 last[6];
        u8 next[6];
        PAD(2);
    };
    size_assert(SData, 0x10);

    SData* mpData;
};
size_assert(MapdataItemPath, 0x4);

class MapdataItemPoint {
public:
    struct SData {
        VEC3 position;
        f32 deviation;
        u16 parameters[2];
    };
    size_assert(SData, 0x14);

    SData* mpData;
    UNK(0x14 - 0x04);
};
size_assert(MapdataItemPoint, 0x14);

class MapdataJugemPoint {
public:
    struct SData {
        VEC3 position;
        VEC3 rotation;
        u16 id;
        s16 range;
    };
    size_assert(SData, 0x1C);

    SData* mpData;

    VEC3 _4;
    VEC3 _10;
    VEC3 _1C;

    u8 enemyPoint;
    u8 itemPoint;

    u16 _2A;
    u16 _2C;
    PAD(2);
};
size_assert(MapdataJugemPoint, 0x30);

class MapdataMissionPoint {
public:
    struct SData {
        VEC3 position;
        VEC3 rotation;
        u16 id;
        PAD(2);
    };
    size_assert(SData, 0x1C);

    SData* mpData;
};
size_assert(MapdataMissionPoint, 0x4);

class MapdataStage {
public:
    struct SData {
        u8 mLapCount;
        bool mPolePosition;
        bool mStartConfig;
        bool mFlareToggle;
        u32 mFlareColor;
        u8 mFlareAlpha;
        u8 _9;
        u16 mSpeedMod; // custom
    };
    size_assert(SData, 0xC);

    SData* mpData;
};
size_assert(MapdataStage, 0x4);

class MapdataStartPoint {
public:
    struct SData {
        VEC3 position;
        VEC3 rotation;
        s16 playerIndex;
        PAD(2);
    };
    size_assert(SData, 0x1C);

    SData* mpData;
    s8 mEnemyPoint;
    PAD(3);
};
size_assert(MapdataStartPoint, 0x8);

class MapdataAreaBase {
public:

    enum Type {
        CAMERA_LINK,
        ENV_EFFECT,
        FOG_SWAP,
        MOVING_ROAD,
        RECALC_DEST,
        MINIMAP_CONTROL,
        BLOOM_SWAP,
        FLYING_BOOS,
        OBJECT_GROUP,
        OBJECT_GROUP_UNLOAD,
        FALL_BOUNDARY,
    };

    struct SData {
        u8 shape;
        u8 type;
        s8 cameraIdx;
        u8 priority;
        VEC3 position;
        VEC3 rotation;
        VEC3 scale;
        u16 parameters[2];
        u8 railID;
        u8 eneLinkID;
        PAD(2);
    };
    size_assert(SData, 0x30);

    virtual bool isInsideShape(const VEC3& pos) const = 0;

    SData* mpData;
    VEC3 mXAxis;
    VEC3 mYAxis;
    VEC3 mZAxis;
    VEC3 mDims;

    f32 mEllipseXRadiusSq;
    f32 mEllipseAspectRatio;
    f32 mBoundingSphereRadiusSq;

    s16 mIndex;
    PAD(2);
};
size_assert(MapdataAreaBase, 0x48);

class MapdataAreaBox : public MapdataAreaBase {
public:
    virtual bool isInsideShape(const VEC3& pos) const;
};
size_assert(MapdataAreaBox, sizeof(MapdataAreaBase));

class MapdataAreaCylinder : public MapdataAreaBase {
public:
    virtual bool isInsideShape(const VEC3& pos) const;
};
size_assert(MapdataAreaCylinder, sizeof(MapdataAreaBase));

class MapdataAreaAccessor {
public:
    MapdataAreaBase** entries;
    u16 numEntries;
    PAD(2);

    virtual ~MapdataAreaAccessor();

    KmpSectionHeader* sectionHeader;
    MapdataAreaBase** byPriority;
};
size_assert(MapdataAreaAccessor, 0x14);

class MapdataCameraAccessor : public MapdataAccessorBase<MapdataCamera> {};
size_assert(MapdataCameraAccessor, 0xC);

class MapdataCannonPointAccessor : public MapdataAccessorBase<MapdataCannonPoint> {};
size_assert(MapdataCannonPointAccessor, 0xC);

class MapdataCheckPathAccessor : public MapdataAccessorBase<MapdataCheckPath> {
public:
    f32 _C;
};
size_assert(MapdataCheckPathAccessor, 0x10);

class MapdataCheckPointAccessor : public MapdataAccessorBase<MapdataCheckPoint> {
public:
    s8 lastKcpId;
    PAD(1);
    u16 finishLineId;
    float meanTotalDistance;
};
size_assert(MapdataCheckPointAccessor, 0x14);

class MapdataEnemyPathAccessor : public MapdataAccessorBase<MapdataEnemyPath> {};
size_assert(MapdataEnemyPathAccessor, 0xC);

class MapdataEnemyPointAccessor : public MapdataAccessorBase<MapdataEnemyPoint> {};
size_assert(MapdataEnemyPointAccessor, 0xC);

class MapdataGeoObjAccessor : public MapdataAccessorBase<MapdataGeoObj> {};
size_assert(MapdataGeoObjAccessor, 0xC);

class MapdataItemPathAccessor : public MapdataAccessorBase<MapdataItemPath> {};
size_assert(MapdataItemPathAccessor, 0xC);

class MapdataItemPointAccessor : public MapdataAccessorBase<MapdataItemPoint> {};
size_assert(MapdataItemPointAccessor, 0xC);

class MapdataJugemPointAccessor : public MapdataAccessorBase<MapdataJugemPoint> {};
size_assert(MapdataJugemPointAccessor, 0xC);

class MapdataMissionPointAccessor : public MapdataAccessorBase<MapdataMissionPoint> {};
size_assert(MapdataMissionPointAccessor, 0xC);

class MapdataPointInfoAccessor : public MapdataAccessorBase<MapdataPointInfo> {};
size_assert(MapdataPointInfoAccessor, 0xC);

class MapdataStageAccessor : public MapdataAccessorBase<MapdataStage> {};
size_assert(MapdataStageAccessor, 0xC);

class MapdataStartPointAccessor : public MapdataAccessorBase<MapdataStartPoint> {};
size_assert(MapdataStartPointAccessor, 0xC);

class MapdataFileAccessor {
public:
    struct SData {
        u32 magic;
        u32 fileSize;
        u16 numSections;
        u16 headerSize;
        u32 revision;
        s32 offsets[];
    };
    size_assert(SData, 0x10);

    SData* mpData;
    u32* mpSectionDef;
    u32 mVersion;
    u32 mSectionDefOffset;
};
size_assert(MapdataFileAccessor, 0x10);

class CourseMap {
public:
    virtual ~CourseMap();
    void init();

    MapdataStageAccessor* parseStageInformation(u32 magic);

    s16 getClosestAreaIdByType(VEC3* pos, s16 prevAreaId, s16 type); // prevAreaId = previous area this function returned

    static const MapdataCannonPoint* getDummyCannonPoint();
    const MapdataCannonPoint* getCannonPoint(u32 id);

    static const MapdataItemPoint* getDummyItemPoint();
    const MapdataItemPoint* getItemPoint(u32 id);

    MapdataFileAccessor* mpCourse;

    MapdataStartPointAccessor* mpStartPoint;
    MapdataEnemyPathAccessor* mpEnemyPath;
    MapdataEnemyPointAccessor* mpEnemyPoint;
    MapdataItemPathAccessor* mpItemPath;
    MapdataItemPointAccessor* mpItemPoint;
    MapdataCheckPathAccessor* mpCheckPath;
    MapdataCheckPointAccessor* mpCheckPoint;
    MapdataPointInfoAccessor* mpPointInfo;
    MapdataGeoObjAccessor* mpGeoObj;
    MapdataAreaAccessor* mpArea;
    MapdataCameraAccessor* mpCamera;
    MapdataJugemPointAccessor* mpJugemPoint;
    MapdataCannonPointAccessor* mpCannonPoint;
    MapdataStageAccessor* mpStageInfo;
    MapdataMissionPointAccessor* mpMissionPoint;

    MapdataCamera* mpGoalCamera;
    MapdataCamera* mpMissionGoalCamera;
    MapdataCamera* mpOpeningCamera;
    u32 _50;

    static CourseMap* instance;
};
size_assert(CourseMap, 0x54);
