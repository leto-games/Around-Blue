#include "GameScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <DrawFunctions/DrawRectangle.hpp>

#include <array>
#include <string>
#include <ranges>

using namespace std;

constexpr int cellCountX = 11;
constexpr int cellCountY = 8;

array<array<int, cellCountX>, cellCountY> matrix{};

static bool isLevelFinished = false;
static bool isLevelFailed = false;

static array<unsigned, 2> cellSelected = {4,5};

GameScene::GameScene(ISceneManager* game) : IScene{game}
{
    LoadLevel();
}

void GameScene::LoadLevel(){
    matrix[4][5] = 1;
    matrix[4][4] = 2;
    matrix[4][6] = 2;
    matrix[3][4] = 3;
    matrix[3][6] = 3;
}

bool GameScene::ProcessInput(const AppEvent& event)
{
    if (IsSystemRightEvent(event)){
        if (cellSelected[1] < cellCountX-1){
            cellSelected = array<unsigned, 2>{cellSelected[0], cellSelected[1] + 1};
            return true;
        }
    }
    if (IsSystemLeftEvent(event)){
        if (cellSelected[1] > 0){
            cellSelected = array<unsigned, 2>{cellSelected[0], cellSelected[1] - 1};
            return true;
        }
    }
    if (IsSystemUpEvent(event)){
        if (cellSelected[0] > 0){
            cellSelected = array<unsigned, 2>{cellSelected[0] - 1, cellSelected[1]};
            return true;
        }
    }
    if (IsSystemDownEvent(event)){
        if (cellSelected[0] < cellCountY-1){
            cellSelected = array<unsigned, 2>{cellSelected[0] + 1, cellSelected[1]};
            return true;
        }
    }

    if (IsSystemAltEvent(event)){
        isLevelFinished = false;
        isLevelFailed = false;
        cellSelected = {4,5};
        LoadLevel();
        return true;
    }
    if (IsSystemEnterEvent(event)){
        // проверка нажатия голубой клетки
        if (matrix[cellSelected[0]][cellSelected[1]] == 1){
            matrix[cellSelected[0]][cellSelected[1]] = 0; // при нажатии на голубой, сделать его черным

            //////////// КРАСНЫЙ ////////////
            if (IsValidMatrix(cellSelected[0], cellSelected[1]-1) && matrix[cellSelected[0]][cellSelected[1]-1] == 2){
                matrix[cellSelected[0]][cellSelected[1]-1] = 1; // красный слева от нажатого голубого, сделать его голубым
            }

            if (IsValidMatrix(cellSelected[0], cellSelected[1]+1) && matrix[cellSelected[0]][cellSelected[1]+1] == 2){
                matrix[cellSelected[0]][cellSelected[1]+1] = 1; // красный справа от нажатого голубого, сделать его голубым
            }

            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]) && matrix[cellSelected[0]-1][cellSelected[1]] == 2){
                matrix[cellSelected[0]-1][cellSelected[1]] = 1; // красный сверху от нажатого голубого, сделать его голубым
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]) && matrix[cellSelected[0]+1][cellSelected[1]] == 2){
                matrix[cellSelected[0]+1][cellSelected[1]] = 1; // красный снизу от нажатого голубого, сделать его голубым
            }

            //////////// ЗЕЛЁНЫЙ ////////////
            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]-1) && matrix[cellSelected[0]-1][cellSelected[1]-1] == 3){
                matrix[cellSelected[0]-1][cellSelected[1]-1] = 2; // зелёный слева сверху от нажатого голубого, сделать его красным
            }

            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]+1) && matrix[cellSelected[0]-1][cellSelected[1]+1] == 3){
                matrix[cellSelected[0]-1][cellSelected[1]+1] = 2; // зелёный справа сверху от нажатого голубого, сделать его красным
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]-1) && matrix[cellSelected[0]+1][cellSelected[1]-1] == 3){
                matrix[cellSelected[0]+1][cellSelected[1]-1] = 2; // зелёный слева снизу от нажатого голубого, сделать его красным
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]+1) && matrix[cellSelected[0]+1][cellSelected[1]+1] == 3){
                matrix[cellSelected[0]+1][cellSelected[1]+1] = 2; // зелёный справа снизу от нажатого голубого, сделать его красным
            }

            //////////// БЕЛЫЙ ////////////
            //////////// ЖЁЛТЫЙ ////////////
            //////////// КОРИЧНЕВЫЙ ////////////
            if (IsValidMatrix(cellSelected[0], cellSelected[1]-1) && matrix[cellSelected[0]][cellSelected[1]-1] == 6){
                if (IsValidMatrix(cellSelected[0], cellSelected[1]-2))
                    matrix[cellSelected[0]][cellSelected[1]-1] = matrix[cellSelected[0]][cellSelected[1]-2]; // коричневый слева от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]][cellSelected[1]-1] = 0;
            }

            if (IsValidMatrix(cellSelected[0], cellSelected[1]+1) && matrix[cellSelected[0]][cellSelected[1]+1] == 6){
                if (IsValidMatrix(cellSelected[0], cellSelected[1]+2))
                    matrix[cellSelected[0]][cellSelected[1]+1] = matrix[cellSelected[0]][cellSelected[1]+2]; // коричневый справа от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]][cellSelected[1]+1] = 0;
            }

            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]) && matrix[cellSelected[0]-1][cellSelected[1]] == 6){
                if (IsValidMatrix(cellSelected[0]-2, cellSelected[1]))
                    matrix[cellSelected[0]-1][cellSelected[1]] = matrix[cellSelected[0]-2][cellSelected[1]]; // коричневый сверху от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]-1][cellSelected[1]] = 0;
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]) && matrix[cellSelected[0]+1][cellSelected[1]] == 6){
                if (IsValidMatrix(cellSelected[0]+2, cellSelected[1]))
                    matrix[cellSelected[0]+1][cellSelected[1]] = matrix[cellSelected[0]+2][cellSelected[1]]; // коричневый снизу от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]+1][cellSelected[1]] = 0;
            }
            //////////// СЕРЫЙ ////////////

            //////////// ПОБЕДА / ПОРАЖЕНИЕ ////////////
            bool isContainsBlue = false;
            bool isContainsOther = false;

            for (const auto& row : matrix) {
                for (int val : row) {
                    if (val != 0 && val != 4) {
                        if (val == 1)
                            isContainsBlue = true;
                        else
                            isContainsOther = true;
                    }
                }
            }

            if (!isContainsBlue && isContainsOther)
                isLevelFailed = true;
            else if (!isContainsBlue && !isContainsOther)
                isLevelFinished = true;
        }
        
        return true;
    }
    return false;
}

void GameScene::Loop(){

}

void GameScene::Draw(IScreen& screen)
{
    static const LetoFont_V1* font = leto_api_v1->Font->GetFont(7, 7, 0);

    Point2_i p1 = {0, 0};
    Point2_i p2 = {0, 0};
    int cellSize = 14;
    int displaySizeX = screen.Width();
    int displaySizeY = screen.Height();

    if (isLevelFailed){
        DrawFunctions::DrawOutlinedRectangle(screen, {30,30}, {130,98}, BlackColor, WhiteColor);
        DrawFunctions::DrawText(screen, { (displaySizeX-42)/2, displaySizeY/2 }, "Провал", RedColor, BlackColor, false, IFont::FromHandle(font));
        return;
    }
    else if (isLevelFinished){
        DrawFunctions::DrawOutlinedRectangle(screen, {30,30}, {130,98}, BlackColor, WhiteColor);
        DrawFunctions::DrawText(screen, { (displaySizeX-49)/2, displaySizeY/2 }, "Победа!", GreenColor, BlackColor, false, IFont::FromHandle(font));
        return;
    }

    for (int x = 0; x < cellCountX; ++x) {
        for (int y = 0; y < cellCountY; ++y) {
            p1 = {x*cellSize+(displaySizeX-cellCountX*cellSize)/2,          y*cellSize+(displaySizeY-cellCountY*cellSize)/2};
            p2 = {x*cellSize+(displaySizeX-cellCountX*cellSize)/2+cellSize, y*cellSize+(displaySizeY-cellCountY*cellSize)/2+cellSize};

            RGBColor outlineColor = BlackColor;
            if (cellSelected == array<unsigned, 2>{(unsigned int)y,(unsigned int)x})
                outlineColor = WhiteColor;

            switch (matrix[y][x])
            {
                case 0:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, BlackColor, outlineColor);
                    break;
                case 1:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, BlueColor, outlineColor);
                    break;
                case 2:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, RedColor, outlineColor);
                    break;
                case 3:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, GreenColor, outlineColor);
                    break;
                case 4:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, WhiteColor, outlineColor);
                    break;
                case 5:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, YellowColor, outlineColor);
                    break;
                case 6:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, BrownColor, outlineColor);
                    break;
                case 7:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, GrayColor, outlineColor);
                    break;
                case 8:
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, aBlueColor, outlineColor);
                    break;
            }
        }
    }

    string coord = "(" + to_string(cellSelected[0]) + ", " + to_string(cellSelected[1]) + ")";
    string type = to_string(matrix[cellSelected[0]][cellSelected[1]]);
    DrawFunctions::DrawText(screen, { 50,0 }, coord.c_str(), coord.length(), WhiteColor, PurpleColor, false, IFont::FromHandle(font));
    DrawFunctions::DrawText(screen, { 25,0 }, type.c_str(), type.length(), WhiteColor, PurpleColor, false, IFont::FromHandle(font));
}

bool GameScene::IsValidMatrix(int y, int x) {
    return y >= 0 && y < cellCountY && x >= 0 && x < cellCountX;
}
