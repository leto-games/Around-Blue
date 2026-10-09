#include "MainScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <DrawFunctions/DrawRectangle.hpp>
#include "../TemplateGame.hpp"

#include <array>

using namespace std;

static int selectedButton = 0;

MainScene::MainScene(ISceneManager* game) : IScene{game}
{

}

void MainScene::Loop(){

}

bool MainScene::ProcessInput(const AppEvent& event){
    if (IsSystemUpEvent(event)){
        if (selectedButton > 0){
            selectedButton--;
            return true;
        }
    }
    if (IsSystemDownEvent(event)){
        if (selectedButton < 2){
            selectedButton++;
            return true;
        }
    }
    if (IsSystemEnterEvent(event)){
        switch (selectedButton){
            case 0:
                scene_manager->SwitchScene((uint8_t) TemplateGame_Scene::LEVELS);
                break;
            case 1:
                // Настройки
                break;
            case 2:
                GameInstance->Close();
                break;
        }
    }
    return false;
}

void MainScene::Draw(IScreen& screen)
{
    static const LetoFont_V1* font = leto_api_v1->Font->GetFont(7, 7, 0);

    int displaySizeX = screen.Width();  // 160
    int displaySizeY = screen.Height(); // 128


    array<StaticText32, 3> menu = {"Уровни",
                             "Настройки",
                             "Выход"};

    for (int i = 0; i < menu.size(); i++){
        DrawFunctions::DrawText(screen, {displaySizeX/2-static_cast<int>(menu[i].TextLength()*1.75f), displaySizeY/2+15+i*15}, menu[i].ConstChar(), menu[i].TextLength(), WhiteColor, BlackColor, false, IFont::FromHandle(font));
    }

    DrawFunctions::DrawText(screen, {displaySizeX/2-45, displaySizeY/2+15+selectedButton*15}, ">", WhiteColor, BlackColor, false, IFont::FromHandle(font));
}