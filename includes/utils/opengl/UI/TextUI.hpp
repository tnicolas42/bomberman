#pragma once

#include "includesOpengl.hpp"
#include "ABaseUI.hpp"

class TextUI : public ABaseUI {
	public:
		TextUI(glm::vec2 pos, glm::vec2 size);
		TextUI(TextUI const & src);
		virtual ~TextUI();

		TextUI & operator=(TextUI const & rhs);

		virtual void	draw();

	protected:
		virtual void	_update();
		TextUI();
};
