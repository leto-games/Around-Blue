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

#include <array>

// Сцена с игрой
class GameScene final : public IScene
{
protected:
    Timer game_timer;
    uint8_t game_part{};

public:
    GameScene(ISceneManager* game);

    // Пользовательский ввод в игру
    virtual bool ProcessInput(const AppEvent& event) override;

    void Loop() override;
    void LoadLevel();

    bool IsValidMatrix(int y, int x);

    // Игровая отрисовка
    virtual void Draw(IScreen& screen) override;

    SCENE_NO_ARGS_BUILDER(GameScene)
};