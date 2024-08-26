#pragma once
#include "GameScene.hpp"

class RaceScene : public GameScene {
public:
    RaceScene();
    virtual ~RaceScene();
    virtual void createSubSystems();

    void loadEffects();
};
