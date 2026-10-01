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

#include <UI/Geometry/UI_Circle.hpp>


// Сцена с игрой
class WelcomeScene final : public IScene
{
protected:
    Timer welcome_timer;
    uint8_t welcome_part{};
    UI_Circle welcome_circle;

public:
    WelcomeScene(ISceneManager* game);

    void Loop() override;

    // Игровая отрисовка
    virtual void Draw(IScreen& screen) override;

    SCENE_NO_ARGS_BUILDER(WelcomeScene)
};