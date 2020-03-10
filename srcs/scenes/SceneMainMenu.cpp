#include "SceneMainMenu.hpp"

SceneMainMenu::SceneMainMenu(Gui * gui, float const &dtTime)
: ASceneMenu(gui, dtTime)
{}

SceneMainMenu::SceneMainMenu(SceneMainMenu const & src)
: ASceneMenu(src)
{
	*this = src;
}

SceneMainMenu::~SceneMainMenu() {}

SceneMainMenu & SceneMainMenu::operator=(SceneMainMenu const & rhs) {
	if (this != &rhs) {
		logWarn("you are copying SceneMainMenu")
	}
	return *this;
}

/**
 * @brief init the menu
 *
 * @return true if the init succed
 * @return false if the init failed
 */
bool			SceneMainMenu::init() {
	addButton(glm::vec2(100, 100), glm::vec2(300, 100), "bouton");
	return true;
}