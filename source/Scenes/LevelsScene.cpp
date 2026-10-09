#include "LevelsScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <DrawFunctions/DrawRectangle.hpp>

#include <array>

#include "GameScene.hpp"

using namespace std;


LevelsScene::LevelsScene(ISceneManager* game) : IScene{game}
{
    
}

void LevelsScene::Loop(){

}

bool LevelsScene::ProcessInput(const AppEvent& event){
    if (IsSystemAltEvent(event)){
        scene_manager->SwitchScene((uint8_t) TemplateGame_Scene::MAIN);
        return true;
    }
    if (IsSystemRightEvent(event)){
        if (selectedLevel[0] < LEVELS_MAX_X){
            selectedLevel[0]++;
        }
        else{
            currentPage++;
        }
        return true;
    }
    if (IsSystemLeftEvent(event)){
        if (selectedLevel[0] > 1){
            selectedLevel[0]--;
        }
        else if (currentPage > 0){
            currentPage--;
        }
        return true;
    }
    if (IsSystemUpEvent(event)){
        if (selectedLevel[1] > 1){
            selectedLevel[1]--;
        }
        return true;
    }
    if (IsSystemDownEvent(event)){
        if (selectedLevel[1] < LEVELS_MAX_Y){
            selectedLevel[1]++;
        }
        return true;
    }
    if (IsSystemEnterEvent(event)){

        GameScene::SetLevelID((selectedLevel[1]-1)*7+selectedLevel[0]+currentPage*LEVELS_MAX_X*LEVELS_MAX_Y);

        scene_manager->SwitchScene((uint8_t) TemplateGame_Scene::GAME);
        // (selectedLevel[1]-1)*7+selectedLevel[0]+currentPage*LEVELS_MAX_X*LEVELS_MAX_Y // порядковый номер уровня
        return true;
    }
    return false;
}

void LevelsScene::Draw(IScreen& screen)
{
    static const LetoFont_V1* font = leto_api_v1->Font->GetFont(7, 7, 0);

    int displaySizeX = screen.Width();  // 160
    int displaySizeY = screen.Height(); // 128
    int levelNumber = currentPage*LEVELS_MAX_X*LEVELS_MAX_Y + 1;

    for (int i = 0; i < LEVELS_MAX_Y; i++){
        for (int j = 0; j < LEVELS_MAX_X; j++){
            StaticText32 level = to_text(levelNumber);
            DrawFunctions::DrawText(screen, {20*j+10, 20*i+10}, level.ConstChar(), level.TextLength());
            levelNumber++;
        }
    }

    DrawFunctions::DrawLine(screen, {selectedLevel[0]*20-10, selectedLevel[1]*20}, {selectedLevel[0]*20+5, selectedLevel[1]*20}, WhiteColor);
}