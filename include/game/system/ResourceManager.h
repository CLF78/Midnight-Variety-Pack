#include <kamek.h>

class ResourceManager {
public:
    void* getFile(int source, char* filename, size_t* size);

    static ResourceManager* instance;
};
