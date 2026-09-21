#include "WelcomeScene.hpp"

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

bool isLevelFinished = false;
bool isLevelFailed = false;

static const RGBColor colors[]
{
    PurpleColor,
    IndigoColor,
    DeepOrangeColor,
    BlueColor,
    DarkGrayColor
};
static const uint32_t colors_cnt = sizeof(colors) / sizeof(colors[0]);

static unsigned cur_color;
static array<unsigned, 2> cellSelected = {4,5};

WelcomeScene::WelcomeScene(ISceneManager* game) : IScene{game}
{
    LoadLevel();
}

void WelcomeScene::LoadLevel(){
    matrix[4][5] = 1;
    matrix[4][4] = 2;
    matrix[4][6] = 2;
    matrix[3][4] = 3;
    matrix[3][6] = 3;
}

bool WelcomeScene::ProcessInput(const AppEvent& event)
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

void WelcomeScene::Loop(){

}

void WelcomeScene::Draw(IScreen& screen)
{
    static const LetoFont_V1* font = leto_api_v1->Font->GetFont(7, 7, 0);

    Point2_i p1 = {0, 0};
    Point2_i p2 = {0, 0};
    int cellSize = 14;
    int displaySizeX = 160;
    int displaySizeY = 128;

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

    // TODO: Убрать все FromHandle
    // DrawFunctions::DrawText(screen, { 0,0 }, "Писька", WhiteColor, colors[cur_color % colors_cnt], true, IFont::FromHandle(font));
    // DrawFunctions::DrawText(screen, { 80,100 }, "жопка", WhiteColor, colors[0 % colors_cnt], false, IFont::FromHandle(font));
    // DrawFunctions::DrawText(screen, { 30,100 }, " ", WhiteColor, colors[cur_color % colors_cnt], false, IFont::FromHandle(font));
    // DrawFunctions::DrawText(screen, { 30,108 }, " ", WhiteColor, colors[cur_color % colors_cnt], false, IFont::FromHandle(font));
    // DrawFunctions::DrawText(screen, { 39,104 }, "    ", WhiteColor, colors[0 % colors_cnt], true, IFont::FromHandle(font)); // 160х128
    // DrawFunctions::DrawText(screen, { 71,104 }, " ", aRedColor, colors[0 % colors_cnt], true, IFont::FromHandle(font));
    
    // int testRound[] = {0,0,0,0,0,0,0,0,0,0,0,
    //                    0,0,0,0,0,0,0,0,0,0,0,
    //                    0,0,0,0,0,0,0,0,0,0,0,
    //                    0,0,0,0,0,0,0,0,0,0,0,
    //                    0,0,0,0,0,0,0,0,0,0,0,
    //                    0,0,0,0,0,0,0,0,0,0,0,
    //                    0,0,0,0,0,0,0,0,0,0,0,
    //                    0,0,0,0,0,0,0,0,0,0,0};

    // for (auto& row : matrix) {
    //     row.fill(1);
    // }

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
    DrawFunctions::DrawText(screen, { 50,0 }, coord.c_str(), coord.length(), WhiteColor, colors[cur_color % colors_cnt], false, IFont::FromHandle(font));
    DrawFunctions::DrawText(screen, { 25,0 }, type.c_str(), type.length(), WhiteColor, colors[cur_color % colors_cnt], false, IFont::FromHandle(font));

    // screen.PixelSet({151,11}, aRedColor);
    // screen.PixelSet({152,12}, aRedColor);
    // screen.PixelSet({153,13}, aRedColor);
    // screen.PixelSet({154,14}, aRedColor);
    // screen.PixelSet({155,15}, aRedColor);
    // screen.PixelSet({156,16}, aRedColor);
    // screen.PixelSet({157,17}, aRedColor);
    // screen.PixelSet({158,18}, aRedColor);
    // screen.PixelSet({159,19}, aRedColor);

    // screen.PixelSet({0,11}, aRedColor);
    // screen.PixelSet({1,12}, aRedColor);
    // screen.PixelSet({2,13}, aRedColor);
    // screen.PixelSet({3,14}, aRedColor);
    // screen.PixelSet({4,15}, aRedColor);
    // screen.PixelSet({5,16}, aRedColor);
    // screen.PixelSet({6,17}, aRedColor);
    // screen.PixelSet({7,18}, aRedColor);
    // screen.PixelSet({8,19}, aRedColor);

    // screen.PixelSet({10,0}, aRedColor);
    // screen.PixelSet({11,1}, aRedColor);
    // screen.PixelSet({12,2}, aRedColor);
    // screen.PixelSet({13,3}, aRedColor);
    // screen.PixelSet({14,4}, aRedColor);
    // screen.PixelSet({15,5}, aRedColor);
    // screen.PixelSet({16,6}, aRedColor);
    // screen.PixelSet({17,7}, aRedColor);
    // screen.PixelSet({18,8}, aRedColor);

    // screen.PixelSet({9,119}, aRedColor);
    // screen.PixelSet({10,120}, aRedColor);
    // screen.PixelSet({11,121}, aRedColor);
    // screen.PixelSet({12,122}, aRedColor);
    // screen.PixelSet({13,123}, aRedColor);
    // screen.PixelSet({14,124}, aRedColor);
    // screen.PixelSet({15,125}, aRedColor);
    // screen.PixelSet({16,126}, aRedColor);
    // screen.PixelSet({17,127}, aRedColor);
}

bool WelcomeScene::IsValidMatrix(int y, int x) {
    return y >= 0 && y < cellCountY && x >= 0 && x < cellCountX;
}
