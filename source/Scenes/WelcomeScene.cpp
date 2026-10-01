#include "WelcomeScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <DrawFunctions/DrawRectangle.hpp>



int displaySizeX = 160;
int displaySizeY = 128;

WelcomeScene::WelcomeScene(ISceneManager* game) : IScene{game}
{
    WelcomeScene::welcome_timer.Start(3000);
}

void WelcomeScene::Loop(){
    if (WelcomeScene::welcome_timer.Expired()){
        scene_manager->SwitchScene((uint8_t) TemplateGame_Scene::MAIN);
    }
}

void WelcomeScene::Draw(IScreen& screen)
{
    static const LetoFont_V1* font = leto_api_v1->Font->GetFont(7, 7, 0);

    int displaySizeX = screen.Width(); // 160
    int displaySizeY = screen.Height(); // 128
    
    DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2-30,displaySizeY/2-30}, {displaySizeX/2,displaySizeY/2}, BlueColor, BlackColor);
    DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2-30,displaySizeY/2}, {displaySizeX/2,displaySizeY/2+30}, RedColor, BlackColor);
    DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2,displaySizeY/2}, {displaySizeX/2+30,displaySizeY/2+30}, GreenColor, BlackColor);
}