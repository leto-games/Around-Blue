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
class MainScene final : public IScene
{
protected:
    Timer main_timer;
    uint8_t main_part{};

public:
    MainScene(ISceneManager* game);
    
    void Loop() override;

    // Игровая отрисовка
    virtual void Draw(IScreen& screen) override;

    SCENE_NO_ARGS_BUILDER(MainScene)
};