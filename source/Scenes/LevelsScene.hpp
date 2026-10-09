#pragma once

#include <SceneManager/ISceneManager.hpp>
#include <GamesSupport/BaseGame.hpp>

#include <UI/Menu/ParamMenu.hpp>
#include <UI/Menu/DialogMenu.hpp>
#include <Time/Timer.hpp>

#include "../Scenes/SceneID.hpp"

#include <Data/StaticList.hpp>
#include <Drawable/DrawableBitmap.hpp>
#include <UI/Animation/MoveAnimation.hpp>
#include <UI/Animation/CartoonAnimation.hpp>
#include <Data/Point2.hpp>


// Сцена с игрой
class LevelsScene final : public IScene
{
protected:
    Timer levels_timer;
    uint8_t levels_part{};
    int selectedLevel[2] = {1,1};
    int LEVELS_MAX_X = 7;
    int LEVELS_MAX_Y = 6;
    int currentPage = 0;

public:
    LevelsScene(ISceneManager* game);
    
    virtual bool ProcessInput(const AppEvent& event) override;
    
    void Loop() override;

    // Игровая отрисовка
    virtual void Draw(IScreen& screen) override;

    SCENE_NO_ARGS_BUILDER(LevelsScene)
};