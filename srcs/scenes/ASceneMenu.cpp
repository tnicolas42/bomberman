#include "ASceneMenu.hpp"
#include "SceneGame.hpp"

// -- Constructors -------------------------------------------------------------

ASceneMenu::ASceneMenu(Gui * gui, float const &dtTime)
: AScene(gui, dtTime), _draw3dMenu(true) {}

ASceneMenu::~ASceneMenu() {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		delete *it;
	}
	_buttons.clear();
}

ASceneMenu::ASceneMenu(ASceneMenu const &src)
: AScene(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

ASceneMenu &ASceneMenu::operator=(ASceneMenu const &rhs) {
	if ( this != &rhs ) {
		logWarn("ASceneMenu object copied");
	}
	return *this;
}

std::ostream &	operator<<(std::ostream & os, const ASceneMenu& myClass) {
	(void)myClass;
	os << "<ASceneMenu object>";
	return os;
}

// -- Methods ------------------------------------------------------------------

/**
 * @brief this is the update function (called every frames)
 *
 * @return true if the update is a success
 * @return false if there are an error in update
 */
bool	ASceneMenu::update() {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		(*it)->update();
	}
	return true;
}

/**
 * @brief this is the draw function (called every frames)
 *
 * @return true if the draw is a success
 * @return false if there are an error in draw
 */
bool	ASceneMenu::draw() {
	bool ret = true;

	/* 3d background */
	if (_draw3dMenu && s.j("debug").b("3d-menu")) {
		SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
		ret = scGame.drawForMenu();
	}
	/* UI elements */
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		(*it)->draw();
	}
	return ret & true;
}
/**
 * @brief called when the scene is loaded
 */
void ASceneMenu::load() {
	_gui->enableCursor(true);
}
/**
 * @brief called when the scene is unloaded
 */
void ASceneMenu::unload() {
}

/**
 * @brief add a button in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param text the text in the button
 * @return ButtonUI& a reference to the element created
 */
ButtonUI & ASceneMenu::addButton(glm::vec2 pos, glm::vec2 size, std::string const & text) {
	ButtonUI * ui = new ButtonUI(pos, size);
	ui->setText(text);
	// set default color
	glm::vec4 color = colorise(s.j("colors").j("buttons").u("color"), s.j("colors").j("buttons").u("alpha"));
	ui->setColor(color);
	ui->setBorderColor(colorise(
		s.j("colors").j("buttons-border").u("color"),
		s.j("colors").j("buttons-border").u("alpha")
	));
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add a button with image in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param filename the path to the image
 * @return ButtonImageUI& a reference to the element created
 */
ButtonImageUI & ASceneMenu::addButtonImage(glm::vec2 pos, glm::vec2 size,
	std::string const & filename)
{
	ButtonImageUI * ui = new ButtonImageUI(pos, size, filename);
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add a slider in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param min min value in slider
 * @param max max value in slider
 * @param val default value in slider
 * @param step step of the slider
 * @return SliderUI& a reference to the element created
 */
SliderUI & ASceneMenu::addSlider(glm::vec2 pos, glm::vec2 size, float min, float max, float val, float step) {
	SliderUI * ui = new SliderUI(pos, size);
	ui->setValues(min, max, val, step);
	// set default color
	glm::vec4 color = colorise(s.j("colors").j("buttons").u("color"), s.j("colors").j("buttons").u("alpha"));
	ui->setColor(color);
	ui->setBorderColor(colorise(
		s.j("colors").j("buttons-border").u("color"),
		s.j("colors").j("buttons-border").u("alpha")
	));
	ui->setTextColor(colorise(
		s.j("colors").j("font").u("color"),
		s.j("colors").j("font").u("alpha")
	));

	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add a text in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param text the text
 * @return TextUI& a reference to the element created
 */
TextUI & ASceneMenu::addText(glm::vec2 pos, glm::vec2 size, std::string const & text) {
	TextUI * ui = new TextUI(pos, size);
	ui->setText(text);
	if (size == VOID_SIZE)
		ui->setCalculatedSize();
	ui->setTextColor(colorise(s.j("colors").j("font").u("color")));
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add a Title in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param text the text
 * @return TextUI& a reference to the element created
 */
TextUI & ASceneMenu::addTitle(glm::vec2 pos, glm::vec2 size, std::string const & text) {
	TextUI * ui = new TextUI(pos, size);
	ui->setText(text);
	ui->setTextFont("title");
	if (size == VOID_SIZE)
		ui->setCalculatedSize();
	ui->setTextColor(colorise(s.j("colors").j("font").u("color")));
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add a rectange in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param color the rectangle color
 * @param borderColor the border rectangle color
 * @return RectUI& a reference to the element created
 */
RectUI & ASceneMenu::addRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color, glm::vec4 borderColor) {
	RectUI * ui = new RectUI(pos, size);
	if (color == VOID_COLOR)
		color = colorise(s.j("colors").j("bg-rect").u("color"), s.j("colors").j("bg-rect").u("alpha"));
	if (borderColor == VOID_COLOR)
		borderColor = colorise(s.j("colors").j("bg-rect-border").u("color"), s.j("colors").j("bg-rect-border").u("alpha"));
	ui->setColor(color);
	ui->setBorderColor(borderColor);
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add an image in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param filename the path to the image
 * @return ImageUI& a reference to the element created
 */
ImageUI & ASceneMenu::addImage(glm::vec2 pos, glm::vec2 size, std::string const & filename) {
	ImageUI * ui = new ImageUI(pos, size, filename);
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add a scrollbar in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @return ScrollbarUI& a reference to the element created
 */
ScrollbarUI & ASceneMenu::addScrollbar(glm::vec2 pos, glm::vec2 size) {
	ScrollbarUI * ui = new ScrollbarUI(pos, size);
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief add a textInput in the menu with menu settings
 *
 * @param pos the position
 * @param size the size
 * @param defText the default text
 * @return TextInputUI& a reference to the element created
 */
TextInputUI & ASceneMenu::addTextInput(glm::vec2 pos, glm::vec2 size, std::string const & defText) {
	TextInputUI * ui = new TextInputUI(pos, size);
	ui->setDefText(defText);
	ui->setTextColor({1, 1, 1, 1});
	ui->setTextAlign(TextAlign::LEFT);
	if (size == VOID_SIZE)
		ui->setCalculatedSize();
	_buttons.push_back(ui);
	return *ui;
}

/**
 * @brief init the basic background of the menu
 *
 * @return true if success
 * @return false if error
 */
bool ASceneMenu::_initBG() {
	if (s.j("debug").b("3d-menu") == false) {
		glm::vec2 winSz = _gui->gameInfo.windowSize;
		glm::vec2 tmpPos = glm::vec2(0, 0);

		addRect(tmpPos, winSz, colorise(
			s.j("colors").j("background").u("color"),
			s.j("colors").j("background").u("alpha")
		));
	}
	return true;
}

/* getter */
// get an UI element (button, slider, ...)
ABaseUI &		ASceneMenu::getUIElement(uint32_t id) { return *_buttons[id]; }
// get the total number of UI elements
uint32_t		ASceneMenu::getNbUIElements() const { return _buttons.size(); }
