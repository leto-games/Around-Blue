#pragma once

#include <cstdint>

enum class TemplateGame_Scene : uint8_t
{
	EXIT,			// Выход из игры
	WELCOME,		// Сцена с приветствием
	MAIN,			// Главная сцена с "Играть", "Настройки" и т.д.
	LEVELS,			// Тестовая подсцена
	GAME,			// Сцена с игрой
};
