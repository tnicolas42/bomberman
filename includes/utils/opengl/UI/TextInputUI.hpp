#pragma once

#include <chrono>
#include "includesOpengl.hpp"
#include "ABaseUI.hpp"

#define CURSOR_SPEED_MS	300

/**
 * @brief this is the UI for text input
 */
class TextInputUI : public ABaseUI {
	public:
		TextInputUI(glm::vec2 pos, glm::vec2 size);
		TextInputUI(TextInputUI const & src);
		virtual ~TextInputUI();

		TextInputUI & operator=(TextInputUI const & rhs);

		TextInputUI &	setDefText(std::string const & defText);
		TextInputUI &	setDefTextColor(glm::vec4 color);
		TextInputUI &	inputInsertText(std::string const & txt);
		TextInputUI &	inputMoveCursor(int move);
		TextInputUI &	inputDelete();
		TextInputUI &	inputSuppr();
		TextInputUI &	inputReset();
		virtual ABaseUI &	setText(std::string const & text);

	protected:
		virtual void	_update();
		virtual void	_draw();
		TextInputUI();

		uint32_t		_getCursorOffset() const;

		/* text */
		std::string					_defText;
		glm::vec4					_defTextColor;

		/* cursor */
		bool						_showCursor;
		std::chrono::milliseconds	_lastShowCursorMs;
		uint32_t					_cursorPos;
};
