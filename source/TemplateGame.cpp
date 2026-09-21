#include "TemplateGame.hpp"

// ----------------------------------------------------------------------------------------------------

TemplateGame::TemplateGame(const AppBinHeader& header) : BaseGame{ header } 
{
}

//#include "Scenes/MainScene.hpp"
//#include "Scenes/PrepareScene.hpp"
//#include "Scenes/GameScene.hpp"
#include "Scenes/SceneID.hpp"

#include "Scenes/WelcomeScene.hpp"
//#include "Scenes/TestScene.hpp"

//#include "Data/GameData.hpp"
//#include "Bitmaps/Bitmaps.hpp"
//#include "Bitmaps/BitmapsWithEffects.hpp"

#include <DrawFunctions/DrawText.hpp>

#include <GamesSupport/Lobby/LobbyScene.hpp>

//using namespace Battleship;

void game_callback(uint32_t id, const void* data, uint32_t size)
{

}

bool TemplateGame::CustomInit()
{
	DrawFunctions::SetDefaultFont(IFont::FromHandle(leto_api_v1->Font->GetFont(8, 8, 0)));

	AddSceneBuilder<WelcomeScene>(TemplateGame_Scene::WELCOME);
	//AddSceneBuilder<MainScene>(Battleship_Scene::MAIN);

	SwitchScene((uint8_t) TemplateGame_Scene::WELCOME);
	return true;
}

void TemplateGame::CustomClose()
{
	//my_field = nullptr;
	//opp_field = nullptr;
}

TemplateGame* GameInstance = nullptr;
