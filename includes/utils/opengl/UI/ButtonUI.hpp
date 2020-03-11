#pragma once

#include "includesOpengl.hpp"
#include "ABaseUI.hpp"

/**
 * @brief this is the UI for button
 */
class ButtonUI : public ABaseUI {
	public:
		ButtonUI(glm::vec2 pos, glm::vec2 size);
		ButtonUI(ButtonUI const & src);
		virtual ~ButtonUI();

		ButtonUI & operator=(ButtonUI const & rhs);

		virtual void	_draw();

	protected:
		virtual void	_update();
		ButtonUI();
};
