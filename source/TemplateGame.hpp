/**
 * @file TemplateGame.hpp
 * @date Aug 28, 2026
 * @author Rakhimov T.
 */

#ifndef INC_TEMPLATE_GAME_HPP_
#define INC_TEMPLATE_GAME_HPP_

#include <GamesSupport/BaseGame.hpp>

class TemplateGame final : public BaseGame<32, 512>
{
public:
	TemplateGame(const AppBinHeader& header);

protected:
	// Инициализация игры
	bool CustomInit() override;

	// Функция завершения игры
	void CustomClose() override;
};

extern TemplateGame* GameInstance;

#endif
