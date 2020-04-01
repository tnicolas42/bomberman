#include "TextInputUI.hpp"
#include "Logging.hpp"
#include "debug.hpp"

TextInputUI::TextInputUI(glm::vec2 pos, glm::vec2 size)
: ABaseUI(pos, size),
  _defText(""),
  _defTextColor(0.3, 0.3, 0.3, 1)
{
	setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	setBorderSize(0);
}

TextInputUI::TextInputUI(TextInputUI const & src): ABaseUI(src) {
	*this = src;
}

TextInputUI::~TextInputUI() {}

TextInputUI & TextInputUI::operator=(TextInputUI const & rhs) {
	(void)rhs;
	// if (this != &rhs) {}
	return *this;
}

/**
 * @brief this is the base update function of UI objects
 */
void TextInputUI::_update() {
}

/**
 * @brief this is the draw function for UI
 * /!\ -> you need to draw in the reverse order (draw at first the element on the top)
 */
void TextInputUI::_draw() {
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;

	// draw text
	std::string textToPrint = (_text == "") ? _defText : _text;
	glm::vec4	textColor = (_text == "") ? _defTextColor : _textColor;
	tmpPos = getRealPos();
	tmpPos.x += _borderSize;
	tmpSize = _size;
	tmpSize.x -= _borderSize * 2;
	_drawText(tmpPos, tmpSize, _textFont, _textScale, textToPrint, textColor, _textAlign, _textPadding);

	// get center size and position
	tmpPos = getRealPos();
	tmpPos.x += _borderSize;
	tmpPos.y += _borderSize;
	tmpSize = _size;
	tmpSize.x -= _borderSize * 2;
	tmpSize.y -= _borderSize * 2;
	_drawRect(tmpPos, tmpSize, _color);

	// draw border
	_drawBorderRect(getRealPos(), _size, _borderSize, _borderColor);
}

TextInputUI & TextInputUI::setDefText(std::string const & defText) { _defText = defText; return *this; }
TextInputUI & TextInputUI::setDefTextColor(glm::vec4 color) { _defTextColor = color; return *this; }