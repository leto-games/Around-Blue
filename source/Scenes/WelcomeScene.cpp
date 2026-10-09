#include "WelcomeScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <DrawFunctions/DrawRectangle.hpp>

using namespace std;

WelcomeScene::WelcomeScene(ISceneManager* game) : IScene{game}
{
    welcome_timer.Start(2000);
}

void WelcomeScene::Loop(){
    if (welcome_timer.Expired()){
        scene_manager->SwitchScene((uint8_t) TemplateGame_Scene::MAIN);
    }
}

void WelcomeScene::Draw(IScreen& screen)
{
    static const LetoFont_V1* font = leto_api_v1->Font->GetFont(7, 7, 0);

    int displaySizeX = screen.Width(); // 160
    int displaySizeY = screen.Height(); // 128

    float timerCount = welcome_timer.GetProgress();
    // string text = to_string(timerCount);

    if (timerCount >= 1){

    }
    else if (timerCount > 0.65){
        DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2,displaySizeY/2}, {displaySizeX/2+30,displaySizeY/2+30}, BlueColor, BlackColor);
    }
    else if (timerCount > 0.35){
        DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2-30,displaySizeY/2}, {displaySizeX/2,displaySizeY/2+30}, BlueColor, BlackColor);
        DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2,displaySizeY/2}, {displaySizeX/2+30,displaySizeY/2+30}, RedColor, BlackColor);
    }
    else if (timerCount > 0.0){
        DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2-30,displaySizeY/2-30}, {displaySizeX/2,displaySizeY/2}, BlueColor, BlackColor);
        DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2-30,displaySizeY/2}, {displaySizeX/2,displaySizeY/2+30}, RedColor, BlackColor);
        DrawFunctions::DrawOutlinedRectangle(screen, {displaySizeX/2,displaySizeY/2}, {displaySizeX/2+30,displaySizeY/2+30}, GreenColor, BlackColor);
    }
    // DrawFunctions::DrawText(screen, {40,0}, text.c_str(), text.length(), WhiteColor, PurpleColor, false, IFont::FromHandle(font));
}