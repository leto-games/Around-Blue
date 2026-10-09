#include "GameScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <DrawFunctions/DrawRectangle.hpp>

#include <array>
#include <ranges>

using namespace std;

int GameScene::level_id = -1;

void GameScene::SetLevelID(int level_id)
{
    GameScene::level_id = level_id;
}

level_t GameScene::GetLevelByID(int level_id)
{
    level_t level_data{};

    StaticText<128> path;
    if (!leto_api_v1->Globals->GetAppDir(path.CharPtr(), path.Capacity()))
        return level_data;

	path += "levels";
    path += "\\level";
    path += to_text(level_id);
    path += ".txt";
	//info_size = leto_api_v1->File->Scan(app_path, info, 128);

	LetoFile_V1* file = leto_api_v1->File->Open(path, LFM_V1_NONE);
    if (!file)
        return level_data;

	int32_t res = leto_api_v1->File->Read(file, level_data.level, sizeof(level_data.level));
	leto_api_v1->File->Close(file);

    return level_data;

	// // }
	// LetoFile_V1* file2 = leto_api_v1->File->Open(app_path + "\\level1.txt", LFM_V1_WRITE);

	// file_info[1] = '1';
	// int32_t r4 = leto_api_v1->File->Write(file2, file_info.CharPtr(), r3);

	// int8_t r5 = leto_api_v1->File->Flush(file2);
}

#include <Input/SystemInputID.hpp>

GameScene::GameScene(ISceneManager* game) : IScene{game}
{
    LoadLevel(GetLevelByID(level_id));
}

void GameScene::LoadLevel(const level_t& level){
    isLevelFinished = false;
    isLevelFailed = false;
    isActionToExit = false;
    cellSelected = {4,5};

    bool isEmpty = true;

    for (int y = 0; y < cellCountY; y++){
        for (int x = 0; x < cellCountX; x++){
            char symbol = level.level[y*(cellCountX+1)+x];
            if (symbol) isEmpty = false;
            matrix[y][x] = symbol;
        }
    }

    if (isEmpty) scene_manager->SwitchScene(TemplateGame_Scene::MAIN);
}

bool GameScene::ProcessInput(const AppEvent& event)
{
    if (IsSystemRightEvent(event)){
        if (isLevelFinished || isLevelFailed){
            isActionToExit = !isActionToExit;
            return true;
        }
        if (cellSelected[1] < cellCountX-1){
            cellSelected = array<unsigned, 2>{cellSelected[0], cellSelected[1] + 1};
            return true;
        }
    }
    if (IsSystemLeftEvent(event)){
        if (isLevelFinished || isLevelFailed){
            isActionToExit = !isActionToExit;
            return true;
        }
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
        cellSelected = {4,5};
        LoadLevel(GetLevelByID(level_id));
        return true;
    }
    if (IsSystemEnterEvent(event)){
        if (isLevelFinished){
            if (!isActionToExit){
                GameScene::SetLevelID(level_id + 1);
                LoadLevel(GetLevelByID(level_id));
            }
            else 
                scene_manager->SwitchScene(TemplateGame_Scene::MAIN);
            return true;
        }
        else if (isLevelFailed){
             if (!isActionToExit){
                LoadLevel(GetLevelByID(level_id));
             }
             else
                scene_manager->SwitchScene(TemplateGame_Scene::MAIN);
            return true;
        }

        // проверка нажатия голубой клетки
        if (matrix[cellSelected[0]][cellSelected[1]] == '1'){
            matrix[cellSelected[0]][cellSelected[1]] = '0'; // при нажатии на голубой, сделать его черным

            //////////// СИНИЙ ////////////
            if (IsValidMatrix(cellSelected[0], cellSelected[1]-1) && matrix[cellSelected[0]][cellSelected[1]-1] == '1'){
                matrix[cellSelected[0]][cellSelected[1]-1] = '3'; // синий слева от нажатого голубого, сделать его зелёным
            }
            if (IsValidMatrix(cellSelected[0], cellSelected[1]+1) && matrix[cellSelected[0]][cellSelected[1]+1] == '1'){
                matrix[cellSelected[0]][cellSelected[1]+1] = '3'; // синий слева от нажатого голубого, сделать его зелёным
            }
            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]) && matrix[cellSelected[0]-1][cellSelected[1]] == '1'){
                matrix[cellSelected[0]-1][cellSelected[1]] = '3'; // синий слева от нажатого голубого, сделать его зелёным
            }
            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]) && matrix[cellSelected[0]+1][cellSelected[1]] == '1'){
                matrix[cellSelected[0]+1][cellSelected[1]] = '3'; // синий слева от нажатого голубого, сделать его зелёным
            }

            //////////// КРАСНЫЙ ////////////
            if (IsValidMatrix(cellSelected[0], cellSelected[1]-1) && matrix[cellSelected[0]][cellSelected[1]-1] == '2'){
                matrix[cellSelected[0]][cellSelected[1]-1] = '1'; // красный слева от нажатого голубого, сделать его голубым
            }

            if (IsValidMatrix(cellSelected[0], cellSelected[1]+1) && matrix[cellSelected[0]][cellSelected[1]+1] == '2'){
                matrix[cellSelected[0]][cellSelected[1]+1] = '1'; // красный справа от нажатого голубого, сделать его голубым
            }

            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]) && matrix[cellSelected[0]-1][cellSelected[1]] == '2'){
                matrix[cellSelected[0]-1][cellSelected[1]] = '1'; // красный сверху от нажатого голубого, сделать его голубым
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]) && matrix[cellSelected[0]+1][cellSelected[1]] == '2'){
                matrix[cellSelected[0]+1][cellSelected[1]] = '1'; // красный снизу от нажатого голубого, сделать его голубым
            }

            //////////// ЗЕЛЁНЫЙ ////////////
            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]-1) && matrix[cellSelected[0]-1][cellSelected[1]-1] == '3'){
                matrix[cellSelected[0]-1][cellSelected[1]-1] = '2'; // зелёный слева сверху от нажатого голубого, сделать его красным
            }

            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]+1) && matrix[cellSelected[0]-1][cellSelected[1]+1] == '3'){
                matrix[cellSelected[0]-1][cellSelected[1]+1] = '2'; // зелёный справа сверху от нажатого голубого, сделать его красным
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]-1) && matrix[cellSelected[0]+1][cellSelected[1]-1] == '3'){
                matrix[cellSelected[0]+1][cellSelected[1]-1] = '2'; // зелёный слева снизу от нажатого голубого, сделать его красным
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]+1) && matrix[cellSelected[0]+1][cellSelected[1]+1] == '3'){
                matrix[cellSelected[0]+1][cellSelected[1]+1] = '2'; // зелёный справа снизу от нажатого голубого, сделать его красным
            }

            //////////// БЕЛЫЙ ////////////
            //////////// ЖЁЛТЫЙ ////////////
            //////////// КОРИЧНЕВЫЙ ////////////
            if (IsValidMatrix(cellSelected[0], cellSelected[1]-1) && matrix[cellSelected[0]][cellSelected[1]-1] == '6'){
                if (IsValidMatrix(cellSelected[0], cellSelected[1]-2))
                    matrix[cellSelected[0]][cellSelected[1]-1] = matrix[cellSelected[0]][cellSelected[1]-2]; // коричневый слева от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]][cellSelected[1]-1] = '0';
            }

            if (IsValidMatrix(cellSelected[0], cellSelected[1]+1) && matrix[cellSelected[0]][cellSelected[1]+1] == '6'){
                if (IsValidMatrix(cellSelected[0], cellSelected[1]+2))
                    matrix[cellSelected[0]][cellSelected[1]+1] = matrix[cellSelected[0]][cellSelected[1]+2]; // коричневый справа от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]][cellSelected[1]+1] = '0';
            }

            if (IsValidMatrix(cellSelected[0]-1, cellSelected[1]) && matrix[cellSelected[0]-1][cellSelected[1]] == '6'){
                if (IsValidMatrix(cellSelected[0]-2, cellSelected[1]))
                    matrix[cellSelected[0]-1][cellSelected[1]] = matrix[cellSelected[0]-2][cellSelected[1]]; // коричневый сверху от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]-1][cellSelected[1]] = '0';
            }

            if (IsValidMatrix(cellSelected[0]+1, cellSelected[1]) && matrix[cellSelected[0]+1][cellSelected[1]] == '6'){
                if (IsValidMatrix(cellSelected[0]+2, cellSelected[1]))
                    matrix[cellSelected[0]+1][cellSelected[1]] = matrix[cellSelected[0]+2][cellSelected[1]]; // коричневый снизу от нажатого голубого, сделать его
                else
                    matrix[cellSelected[0]+1][cellSelected[1]] = '0';
            }
            //////////// СЕРЫЙ ////////////

            //////////// ПОБЕДА / ПОРАЖЕНИЕ ////////////
            bool isContainsBlue = false;
            bool isContainsOther = false;

            for (const auto& row : matrix) {
                for (int val : row) {
                    if (val != '0' && val != '4') {
                        if (val == '1')
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
        DrawFunctions::DrawText(screen, { (displaySizeX-42)/2, displaySizeY/2-10 }, "Провал", RedColor, BlackColor, false, IFont::FromHandle(font));
        DrawFunctions::DrawText(screen, { (displaySizeX-35)/2-25, displaySizeY/2+20 }, "Выйти", WhiteColor, BlackColor, false, IFont::FromHandle(font));
        DrawFunctions::DrawText(screen, { (displaySizeX-35)/2+25+2, displaySizeY/2+20 }, "Снова", WhiteColor, BlackColor, false, IFont::FromHandle(font));

        if (!isActionToExit){
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2-25-2, displaySizeY/2+20}, {(displaySizeX-35)/2+10, displaySizeY/2+27}, RedColor);
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2-25-2, displaySizeY/2+27}, {(displaySizeX-35)/2+10, displaySizeY/2+20}, RedColor);
        }
        else{
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2+25+2, displaySizeY/2+20}, {(displaySizeX-35)/2+60+2, displaySizeY/2+27}, RedColor);
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2+25+2, displaySizeY/2+27}, {(displaySizeX-35)/2+60+2, displaySizeY/2+20}, RedColor);
        }
        return;
    }
    else if (isLevelFinished){
        DrawFunctions::DrawOutlinedRectangle(screen, {30,30}, {130,98}, BlackColor, WhiteColor);
        DrawFunctions::DrawText(screen, { (displaySizeX-42)/2, displaySizeY/2-10}, "Победа", GreenColor, BlackColor, false, IFont::FromHandle(font));
        DrawFunctions::DrawText(screen, { (displaySizeX-35)/2-25, displaySizeY/2+20 }, "Выйти", WhiteColor, BlackColor, false, IFont::FromHandle(font));
        DrawFunctions::DrawText(screen, { (displaySizeX-35)/2+25+2, displaySizeY/2+20 }, "Далее", WhiteColor, BlackColor, false, IFont::FromHandle(font));

        if (!isActionToExit){
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2-25-2, displaySizeY/2+20}, {(displaySizeX-35)/2+10, displaySizeY/2+27}, RedColor);
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2-25-2, displaySizeY/2+27}, {(displaySizeX-35)/2+10, displaySizeY/2+20}, RedColor);
        }
        else{
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2+25+2, displaySizeY/2+20}, {(displaySizeX-35)/2+60+2, displaySizeY/2+27}, RedColor);
            DrawFunctions::DrawLine(screen, {(displaySizeX-35)/2+25+2, displaySizeY/2+27}, {(displaySizeX-35)/2+60+2, displaySizeY/2+20}, RedColor);
        }
        return;
    }

    for (int x = 0; x < cellCountX; ++x) {
        for (int y = 0; y < cellCountY; ++y) {
            p1 = {x*cellSize+(displaySizeX-cellCountX*cellSize)/2,          y*cellSize+(displaySizeY-cellCountY*cellSize)/2};
            p2 = {x*cellSize+(displaySizeX-cellCountX*cellSize)/2+cellSize, y*cellSize+(displaySizeY-cellCountY*cellSize)/2+cellSize};

            RGBColor outlineColor = BlackColor;

            switch (matrix[y][x])
            {
                case '0':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, BlackColor, outlineColor);
                    break;
                case '1':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, BlueColor, outlineColor);
                    break;
                case '2':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, RedColor, outlineColor);
                    break;
                case '3':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, GreenColor, outlineColor);
                    break;
                case '4':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, WhiteColor, outlineColor);
                    break;
                case '5':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, YellowColor, outlineColor);
                    break;
                case '6':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, BrownColor, outlineColor);
                    break;
                case '7':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, GrayColor, outlineColor);
                    break;
                case '8':
                    DrawFunctions::DrawOutlinedRectangle(screen, p1, p2, aBlueColor, outlineColor);
                    break;
            }
        }
    }

    leto_api_v1->Graphics->DrawRect(IScreen::ToHandle(&screen), 3+cellSelected[1]*14, 8+cellSelected[0]*14, 15, 15, 1, WhiteColor);

    // string coord = "(" + to_string(cellSelected[0]) + ", " + to_string(cellSelected[1]) + ")";
    // string type = to_string(matrix[cellSelected[0]][cellSelected[1]]);
    // DrawFunctions::DrawText(screen, { 50,0 }, coord.c_str(), coord.length(), WhiteColor, PurpleColor, false, IFont::FromHandle(font));
    // DrawFunctions::DrawText(screen, { 25,0 }, type.c_str(), type.length(), WhiteColor, PurpleColor, false, IFont::FromHandle(font));

    StaticText32 level = "Level: ";
    level += to_text(GameScene::level_id);

    static const LetoFont_V1* levelFont = leto_api_v1->Font->GetFont(7, 7, 1);
    DrawFunctions::DrawText(screen, { 10,0 }, level.ConstChar(), level.TextLength(), WhiteColor, BlackColor, false, IFont::FromHandle(levelFont));
}

bool GameScene::IsValidMatrix(int y, int x) {
    return y >= 0 && y < cellCountY && x >= 0 && x < cellCountX;
}
