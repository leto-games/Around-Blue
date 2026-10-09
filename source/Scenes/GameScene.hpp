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

#include <Input/Catchers/ButtonCatcher.hpp>

#include <array>

using namespace std;

struct level_t
{
    char level[96];
};


// Сцена с игрой
class GameScene final : public IScene
{
protected:
    Timer game_timer;
    uint8_t game_part{};

    static int level_id;

    ButtonCatcher<GameScene> menu_catcher;

    static constexpr int cellCountX = 11;
    static constexpr int cellCountY = 8;

    array<array<char, cellCountX>, cellCountY> matrix{};

    bool isLevelFinished = false;
    bool isLevelFailed = false;

    bool isActionToExit = false;

    array<unsigned, 2> cellSelected = {4,5};

public:

    GameScene(ISceneManager* game);

    // Пользовательский ввод в игру
    virtual bool ProcessInput(const AppEvent& event) override;

    void Loop() override;
    void LoadLevel(const level_t& level);

    bool IsValidMatrix(int y, int x);

    // Игровая отрисовка
    virtual void Draw(IScreen& screen) override;

    static void SetLevelID(int level_id);

    level_t GetLevelByID(int level_id);

    SCENE_NO_ARGS_BUILDER(GameScene)
};
