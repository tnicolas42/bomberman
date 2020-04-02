#include <cctype>
#include "TextInputUI.hpp"
#include "Logging.hpp"
#include "debug.hpp"

TextInputUI::TextInputUI(glm::vec2 pos, glm::vec2 size)
: ABaseUI(pos, size),
  _isAlwaysFocus(false),
  _looseFocusNextTime(false),
  _defText(""),
  _defTextColor(0.3, 0.3, 0.3, 1),
  _showCursor(false),
  _lastShowCursorMs(0),
  _cursorPos(0)
{
	setFocus(false);
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
	/* update focus */
	if (!_isAlwaysFocus) {
		if (_looseFocusNextTime) {
			_looseFocusNextTime = false;
			setFocus(false);
		}
		if (_hasFocus) {
			if (!_leftClick && Inputs::getLeftClickDown()) {
				setFocus(false);
			}
		}
		else {
			if (_leftClick) {
				setFocus(true);
			}
		}
	}
	else {
		setFocus(true);
	}

	/* don't update if has ot focus */
	if (!_hasFocus) {
		_showCursor = false;
		return;
	}

	setFocus(false);
	if (Inputs::getKeyUp(InputType::CANCEL)) {
		if (_isAlwaysFocus)  // loose focus for a frame only
			return;
		_looseFocusNextTime = true;
	}
	else if (Inputs::getKeyByScancodeUp(SDL_SCANCODE_RETURN)) {
		return;  // loose focus (for one frame if _isAlwaysFocus)
	}
	setFocus(true);

	/* update text */
	if (Inputs::getTextInputString() != "") {
		inputInsertText(Inputs::getTextInputString());
	}
	switch (Inputs::getTextInputKeycode()) {
		case SDLK_BACKSPACE:
			inputDelete();
			break;
		case SDLK_DELETE:
			inputSuppr();
			break;
		case SDLK_LEFT:
			inputMoveCursor(-1);
			break;
		case SDLK_RIGHT:
			inputMoveCursor(1);
			break;
		default:
			break;
	}


	/* cursor blink */
	std::chrono::milliseconds curMs = std::chrono::duration_cast<std::chrono::milliseconds>(
										std::chrono::system_clock::now().time_since_epoch());
	if ((curMs - _lastShowCursorMs).count() > CURSOR_SPEED_MS) {
		_lastShowCursorMs = curMs;
		_showCursor = !_showCursor;
	}
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
	if (_showCursor) {
		tmpPos.x += _getCursorOffset();
		_drawText(tmpPos, tmpSize, _textFont, _textScale, "|", _textColor, _textAlign, _textPadding);
	}

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

/**
 * @brief TO set if the textInput has always focus or only when clicking
 *
 * @param isAlwaysFocus Has always focus ?
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::setAlwaysFocus(bool isAlwaysFocus) {
	_isAlwaysFocus = isAlwaysFocus;
	return *this;
}
/**
 * @brief Set default text in TextInput (help text when input is empty)
 *
 * @param defText The text
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::setDefText(std::string const & defText) { _defText = defText; return *this; }
/**
 * @brief Set the color of the default text for textInput (see TextInputUI::setDefText)
 *
 * @param color The text color
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::setDefTextColor(glm::vec4 color) { _defTextColor = color; return *this; }

/**
 * @brief Insert text in TextInput (at cursor position)
 *
 * @param txt The text to insert
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::inputInsertText(std::string const & txt) {
	if (_cursorPos == 0) {
		_text = txt + _text;
	}
	else if (_cursorPos == _text.size()) {
		_text = _text + txt;
	}
	else {
		_text = _text.substr(0, _cursorPos) + txt + _text.substr(_cursorPos, _text.size() - _cursorPos);
	}
	inputMoveCursor(txt.size());
	return *this;
}
/**
 * @brief Move the cursor for TextInput
 *
 * @param move The moving (negative to move left)
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::inputMoveCursor(int move) {
	if (move < 0) {
		if (static_cast<uint32_t>(-move) >= _cursorPos)
			_cursorPos = 0;
		else
			_cursorPos += move;
	}
	else {
		_cursorPos += move;
		if (_cursorPos > _text.size()) {
			_cursorPos = _text.size();
		}
	}
	return *this;
}
/**
 * @brief Delete the char before cursor in TextInput
 *
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::inputDelete() {
	if (_cursorPos > 0) {
		_text = _text.substr(0, _cursorPos - 1) + _text.substr(_cursorPos, _text.size() - _cursorPos);
		inputMoveCursor(-1);
	}
	return *this;
}
/**
 * @brief Delete the char after cursor in TextInput
 *
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::inputSuppr() {
	if (_cursorPos < _text.size()) {
		_text = _text.substr(0, _cursorPos) + _text.substr(_cursorPos + 1, _text.size() - _cursorPos);
	}
	return *this;
}
/**
 * @brief Reset the text input
 *
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::inputReset() {
	_text = "";
	_cursorPos = 0;
	return *this;
}

/**
 * @brief Enable/disable focus for TextInput
 *
 * @param focus Enable / disable
 * @return TextInputUI& A reference to the UI object
 */
TextInputUI & TextInputUI::setFocus(bool focus) {
	if (focus == _hasFocus)
		return *this;
	_hasFocus = focus;
	Inputs::setTextInputMode(_hasFocus);
	return *this;
}
/**
 * @brief Redefinition of ABaseUI::setText function
 *
 * @param txt The new text
 * @return ABaseUI& A reference to the UI object
 */
ABaseUI & TextInputUI::setText(std::string const & txt) {
	inputReset();
	inputInsertText(txt);
	return *this;
}

uint32_t TextInputUI::_getCursorOffset() const {
	if (_text.size() == 0 || _cursorPos == 0)
		return 0;
	return _textRender->strWidth(_textFont, _text.substr(0, _cursorPos), _textScale)
		- (_textRender->strWidth(_textFont, "|", _textScale) / 2);
}
