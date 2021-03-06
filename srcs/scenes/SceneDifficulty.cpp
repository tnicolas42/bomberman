#include "SceneDifficulty.hpp"
#include "AudioManager.hpp"
#include "Save.hpp"

/**
 * @brief Construct a new Scene Difficulty:: Scene Difficulty object
 *
 * @param gui A pointer on the gui object
 * @param dtTime A reference to the delta time
 */
SceneDifficulty::SceneDifficulty(Gui * gui, float const &dtTime)
: ASceneMenu(gui, dtTime)
{}

/**
 * @brief Construct a new Scene Difficulty:: Scene Difficulty object
 *
 * @param src The object to do the copy
 */
SceneDifficulty::SceneDifficulty(SceneDifficulty const & src)
: ASceneMenu(src)
{
	*this = src;
}

/**
 * @brief Destroy the Scene Difficulty:: Scene Difficulty object
 */
SceneDifficulty::~SceneDifficulty() {}

/**
 * @brief Copy this object
 *
 * @param rhs The object to copy
 * @return SceneDifficulty& A reference to the copied object
 */
SceneDifficulty & SceneDifficulty::operator=(SceneDifficulty const & rhs) {
	if (this != &rhs) {
		logWarn("you are copying SceneDifficulty")
	}
	return *this;
}

// - AScene Public Methods -----------------------------------------------------

/**
 * @brief init the menu
 *
 * @return true if the init succeed
 * @return false if the init failed
 */
bool			SceneDifficulty::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;

	try {
		addExitButton()
			.addButtonLeftListener(&_states.menu);

		tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;
		tmpSize.x = menuWidth;
		tmpSize.y = menuHeight;
		addTitle(tmpPos, tmpSize, "Game mode");

		allUI.beginner = &addButton(VOID_SIZE, VOID_SIZE, "beginner")
			.setKeyLeftClickScancode(SDL_SCANCODE_0)
			.addButtonLeftListener(&_states.beginner);
		allUI.easy = &addButton(VOID_SIZE, VOID_SIZE, "easy")
			.setKeyLeftClickScancode(SDL_SCANCODE_1)
			.addButtonLeftListener(&_states.easy);
		allUI.medium = &addButton(VOID_SIZE, VOID_SIZE, "medium")
			.setKeyLeftClickScancode(SDL_SCANCODE_2)
			.addButtonLeftListener(&_states.medium);
		allUI.hardCore = &addButton(VOID_SIZE, VOID_SIZE, "hard core")
			.setKeyLeftClickScancode(SDL_SCANCODE_3)
			.addButtonLeftListener(&_states.hardCore);

		allUI.border = &addRect(VOID_SIZE, VOID_SIZE);

		_initBG();
	}
	catch (ABaseUI::UIException const & e) {
		logErr(e.what());
		return false;
	}
	return true;
}

/**
 * @brief called when the scene is loaded
 */
void SceneDifficulty::load() {
	ASceneMenu::load();
	_updateUI();
}

/**
 * @brief this is the update function (called every frames)
 *
 * @return true if the update is a success
 * @return false if there are an error in update
 */
bool	SceneDifficulty::update() {
	ASceneMenu::update();
	if (_states.beginner) {
		_states.beginner = false;
		Save::newGame();
		Save::setDifficulty(10);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.easy) {
		_states.easy = false;
		Save::newGame();
		Save::setDifficulty(3);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.medium) {
		_states.medium = false;
		Save::newGame();
		Save::setDifficulty(2);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.hardCore) {
		_states.hardCore = false;
		Save::newGame();
		Save::setDifficulty(1);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.menu || Inputs::getKeyUp(InputType::CANCEL)) {
		_states.menu = false;
		SceneManager::loadScene(SceneNames::MAIN_MENU);
	}
	return true;
}

// -- Private methods ----------------------------------------------------------

/**
 * @brief Update UI objects.
 *
 */
void		SceneDifficulty::_updateUI() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;
	tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;
		tmpSize.x = menuWidth;
		tmpSize.y = menuHeight;
	tmpPos.y -= menuHeight * 1.8;
	allUI.beginner->setPos(tmpPos).setSize(tmpSize);
	tmpPos.y -= menuHeight * 1.3;
	allUI.easy->setPos(tmpPos).setSize(tmpSize);
	tmpPos.y -= menuHeight * 1.3;
	allUI.medium->setPos(tmpPos).setSize(tmpSize);
	tmpPos.y -= menuHeight * 1.3;
	allUI.hardCore->setPos(tmpPos).setSize(tmpSize);
	tmpSize.x = tmpSize.x * 1.3;
	tmpSize.y = winSz.y - tmpPos.y - menuHeight * 1.8;
	tmpPos.x = (winSz.x / 2) - ((menuWidth * 1.3) / 2);
	tmpPos.y -= menuHeight * 0.5;
	allUI.border->setPos(tmpPos).setSize(tmpSize);
}
