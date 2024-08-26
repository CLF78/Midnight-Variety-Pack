#pragma once
#include "InitScene.hpp"

class GameScene : public InitScene {
public:
    GameScene();
    virtual ~GameScene();
    virtual void calc();
    virtual void draw();
    virtual void enter();
    virtual void exit();
    virtual void reinit();
    virtual bool isSpectating();
    virtual void createSubSystems();
};
