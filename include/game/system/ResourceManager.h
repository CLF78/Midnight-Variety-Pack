#include <kamek.h>
#include <game/system/MultiDvdArchive.h>

class ResourceManager {
public:
    void* getFile(int source, char* filename, size_t* size);
    void preloadCourseAsync(u32 courseId);

    MultiDvdArchive* loadCourse(u32 courseId, EGG::Heap* heap, bool isSplitScreen);
    MultiDvdArchive* loadMission(u32 courseId, u32 missionNum, EGG::Heap* heap, bool isSplitScreen);

    static ResourceManager* instance;
};
