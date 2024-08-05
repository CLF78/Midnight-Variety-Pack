#pragma once

namespace EGG {
    
class ColorFader;
class Scene;

class SceneManager {
public:
    virtual void calc();
    virtual void draw();
    virtual void calcCurrentScene();
    virtual void calcCurrentFader();
    virtual void drawCurrentScene();
    virtual void drawCurrentFader();
    virtual void createDefaultFader();

    void* creator;
    u32 _8;

    Scene* currentScene;
    SceneManager* parent;
    int nextSceneId;
    int _18, _1C;

    int transitionStatus;
    ColorFader* currentFader;
    u32 useMem2;
    void* nextCreator;
};
size_assert(SceneManager, 0x30);

} // namespace EGG
