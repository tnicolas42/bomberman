#include "SceneEndGame.hpp"
#include "AudioManager.hpp"
#include "Save.hpp"

/**
 * @brief Construct a new Scene End Game:: Scene End Game object
 *
 * @param gui A pointer on the gui object
 * @param dtTime A reference to the delta time
 */
SceneEndGame::SceneEndGame(Gui * gui, float const &dtTime)
: ASceneMenu(gui, dtTime)
{
	_draw3dMenu = false;
	AudioManager::loadMusic("sounds/the_offspring-the_kids_arent_alright.ogg");
}

/**
 * @brief Construct a new Scene End Game:: Scene End Game object
 *
 * @param src The object to do the copy
 */
SceneEndGame::SceneEndGame(SceneEndGame const & src)
: ASceneMenu(src)
{
	*this = src;
}

/**
 * @brief Destroy the Scene End Game:: Scene End Game object
 */
SceneEndGame::~SceneEndGame() {}

/**
 * @brief Copy this object
 *
 * @param rhs The object to copy
 * @return SceneEndGame& A reference to the copied object
 */
SceneEndGame & SceneEndGame::operator=(SceneEndGame const & rhs) {
	if (this != &rhs) {
		logWarn("you are copying SceneEndGame")
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
bool			SceneEndGame::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x * 0.8;
	float menuHeight = winSz.y / 14;

	try {
		addExitButton()
			.addButtonLeftListener(&_states.exit);

		tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;
		tmpSize.x = menuWidth;
		tmpSize.y = menuHeight;
		glm::vec2 imgSize = {menuWidth, menuWidth * 350 / 800};
		tmpPos.y -= imgSize.y * 0.8;
		addImage(tmpPos, imgSize, s.s("logo"));

		tmpPos.x = winSz.x / 2 - menuWidth / 2;
		tmpPos.y += imgSize.y * 0.1;
		addText(tmpPos, {menuWidth, menuHeight}, "Thank's you for playing !")
			.setTextAlign(TextAlign::CENTER);

		tmpPos.y = menuHeight / 2;
		float widthText = winSz.x / 4;

		tmpPos.x = 0;
		addText(tmpPos, {widthText, menuHeight}, "Ernest  Marin")
			.setTextColor(colorise(s.j("colors").j("black").u("color"), s.j("colors").j("black").u("alpha")))
			.setTextAlign(TextAlign::CENTER);

		tmpPos.x = 1 * widthText;
		addText(tmpPos, {widthText, menuHeight}, "Emilien  Baudet")
			.setTextColor(colorise(s.j("colors").j("black").u("color"), s.j("colors").j("black").u("alpha")))
			.setTextAlign(TextAlign::CENTER);

		tmpPos.x = 2 * widthText;
		addText(tmpPos, {widthText, menuHeight}, "Tim  Nicolas")
			.setTextColor(colorise(s.j("colors").j("black").u("color"), s.j("colors").j("black").u("alpha")))
			.setTextAlign(TextAlign::CENTER);

		tmpPos.x = 3 * widthText;
		addText(tmpPos, {widthText, menuHeight}, "Guilhem  Smith")
			.setTextColor(colorise(s.j("colors").j("black").u("color"), s.j("colors").j("black").u("alpha")))
			.setTextAlign(TextAlign::CENTER);

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
void SceneEndGame::load() {
	ASceneMenu::load();
	AudioManager::playMusic("sounds/the_offspring-the_kids_arent_alright.ogg", 0.3f, true);
}

/**
 * @brief this is the update function (called every frames)
 *
 * @return true if the update is a success
 * @return false if there are an error in update
 */
bool	SceneEndGame::update() {
	ASceneMenu::update();
	SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
	if (s.j("debug").b("3d-menu")) {
		scGame.updateForMenu();
	}

	if (_states.exit || Inputs::getKeyUp(InputType::ACTION)) {
		_states.exit = false;
		AudioManager::playMusic(MUSIC_MENU, 0.1f, true);
		SceneManager::loadScene(SceneNames::MAIN_MENU);
	}
	return true;
}

/**
 * @brief this is the draw function (called every frames)
 *
 * @return true if the draw is a success
 * @return false if there are an error in draw
 */
bool SceneEndGame::draw() {
	bool ret = true;

	/* 3d background */
	if (s.j("debug").b("3d-menu")) {
		SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
		ret = scGame.drawEndGame();  // draw the game if possible
	}
	ret = ASceneMenu::draw();
	return ret & true;
}
