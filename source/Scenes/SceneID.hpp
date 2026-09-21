/**
 * @file SceneID.hpp
 * @date Aug 28, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SCENES_SCENE_ID_HPP_
#define INC_SCENES_SCENE_ID_HPP_

#include <cstdint>

enum class TemplateGame_Scene : uint8_t
{
	EXIT,			// Выход из игры
	WELCOME,		// Сцена с приветствием
	MAIN,			// Главная сцена с "Играть", "Настройки" и т.д.
	TEST,			// Тестовая подсцена
};

#endif
