/*
 * MainScene.hpp
 *
 *  Created on: Dec 13, 2025
 *      Author: Timur
 */

#ifndef INC_GAME_BATTLESHIP_MAIN_SCENE_HPP_
#define INC_GAME_BATTLESHIP_MAIN_SCENE_HPP_

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

// Сцена с приветствием
class WelcomeScene final : public IScene
{
protected:
    Timer welcome_timer;
    uint8_t welcome_part{};

public:
    WelcomeScene(ISceneManager* game);

    // Пользовательский ввод в игру
    virtual bool ProcessInput(const AppEvent& event) override;

    void Loop() override;
    void LoadLevel();

    bool IsValidMatrix(int y, int x);

    // Игровая отрисовка
    virtual void Draw(IScreen& screen) override;

    SCENE_NO_ARGS_BUILDER(WelcomeScene)
};

#endif
