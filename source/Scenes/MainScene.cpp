#include "MainScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <DrawFunctions/DrawRectangle.hpp>

MainScene::MainScene(ISceneManager* game) : IScene{game}
{

}

void MainScene::Loop(){

}

void MainScene::Draw(IScreen& screen)
{
    static const LetoFont_V1* font = leto_api_v1->Font->GetFont(7, 7, 0);

    Point2_i p1 = {0, 0};
    Point2_i p2 = {0, 0};

    // DrawFunctions::DrawOutlinedRectangle(screen, {30,30}, {130,98}, BlackColor, WhiteColor);
    DrawFunctions::DrawRectangle(screen, {30,30}, {130,98}, WhiteColor);
}