#include "ImageUI.hpp"
#include "Logging.hpp"
#include "debug.hpp"
#include "Texture.hpp"

ImageUI::ImageUI(glm::vec2 pos, glm::vec2 size, std::string const & filename, bool pixelateOnZoom)
: ABaseUI(pos, size)
{
	// disable color
	_color = glm::vec4(0.0, 0.0, 0.0, 0.0);
	_borderSize = 0;

	// load the image
	_loadImg(filename, true, pixelateOnZoom);
}

ImageUI::ImageUI(ImageUI const & src): ABaseUI(src) {
	*this = src;
}

ImageUI::~ImageUI() {}

ImageUI & ImageUI::operator=(ImageUI const & rhs) {
	(void)rhs;
	// if (this != &rhs) {}
	return *this;
}

/**
 * @brief this is the base update function of UI objects
 *
 * @param mousePos the position of the mouse
 * @param rightClick a boolean to know if right click is pressed
 * @param leftClick a boolean to know if left click is pressed
 */
void ImageUI::_update(glm::vec2 mousePos, bool rightClick, bool leftClick) {
	(void)mousePos;
	(void)rightClick;
	(void)leftClick;
}

/**
 * @brief this is the draw function for UI
 * /!\ -> you need to draw in the reverse order (draw at first the element on the top)
 */
void ImageUI::draw() {
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;

	// draw border
	_drawBorderRect(_pos, _size, _borderSize, _borderColor);

	// get center size and position
	tmpPos = _pos;
	tmpPos.x += _borderSize;
	tmpPos.y += _borderSize;
	tmpSize = _size;
	tmpSize.x -= _borderSize * 2;
	tmpSize.y -= _borderSize * 2;
	// draw image
	_drawImg(tmpPos, tmpSize, _imgTextureID, _color);
}