#include <unistd.h>
#include "SceneLoading.hpp"
#include "AudioManager.hpp"
#include "Save.hpp"
#include "FileUtils.hpp"

/**
 * @brief Construct a new Scene Loading:: Scene Loading object
 *
 * @param gui A pointer on the gui object
 * @param dtTime A reference to the delta time
 */
SceneLoading::SceneLoading(Gui * gui, float const &dtTime)
: ASceneMenu(gui, dtTime)
{
	_draw3dMenu = false;
	_blurEnabled = false;
}

/**
 * @brief Construct a new Scene Loading:: Scene Loading object
 *
 * @param src The object to do the copy
 */
SceneLoading::SceneLoading(SceneLoading const & src)
: ASceneMenu(src)
{
	*this = src;
}

/**
 * @brief Destroy the Scene Loading:: Scene Loading object
 */
SceneLoading::~SceneLoading() {}

/**
 * @brief Copy this object
 *
 * @param rhs The object to copy
 * @return SceneLoading& A reference to the copied object
 */
SceneLoading & SceneLoading::operator=(SceneLoading const & rhs) {
	if (this != &rhs) {
		logWarn("you are copying SceneLoading")
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
bool			SceneLoading::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x * 0.8;
	float menuHeight = winSz.y / 14;

	try {
		tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;
		tmpSize.x = menuWidth;
		tmpSize.y = menuHeight;
		glm::vec2 imgSize = {menuWidth, menuWidth * 350 / 800};
		tmpPos.y -= imgSize.y;
		addImage(tmpPos, imgSize, s.s("logo"));

		/* background image */
		std::vector<std::string> allImgs = file::ls(s.s("loadingImgs"));
		if (allImgs.empty()) {
			logWarn("No image available for loading menu");
		}
		else {
			int imgID = rand() % allImgs.size();
			try {
				ABaseUI * tmp = &addImage({0, 0}, {0, 0}, allImgs[imgID]);
				if (winSz.x / winSz.y > static_cast<float>(tmp->getImgDefSize().x) / tmp->getImgDefSize().y) {
					tmpSize.x = winSz.x;
					tmpSize.y = winSz.x / (static_cast<float>(tmp->getImgDefSize().x) / tmp->getImgDefSize().y);
				}
				else {
					tmpSize.x = winSz.y * (static_cast<float>(tmp->getImgDefSize().x) / tmp->getImgDefSize().y);
					tmpSize.y = winSz.y;
				}
				tmp->setSize(tmpSize);
			}
			catch (ABaseUI::UIException const & e) {
				logWarn("Invalid image " << allImgs[imgID] << " in loading scene");
			}
		}

		/* load sentence */
		std::ifstream	file(s.s("loadingSentences"));
		std::string		line;
		std::vector<std::string> allSentences;
		if (file.is_open()) {
			for (std::string line; std::getline(file, line); ) {
				if (line.size() > 0)
					allSentences.push_back(line);
			}
			file.close();
		}
		else {
			logWarn("unable to load sentences list for loading menu");
		}
		if (allSentences.empty()) {
			logWarn("No sentences in sentences files (" << s.s("loadingSentences") << ")");
			allSentences.push_back("");
		}
		/* loading text */
		tmpPos.x = winSz.x / 2;
		tmpPos.y = 100;
		addText(tmpPos, {0, 0}, "loading...")
			.setTextAlign(TextAlign::CENTER)
			.setZ(1);
		int sentenceID = rand() % allSentences.size();

		/* loading sentence */
		tmpSize.x = winSz.x;
		tmpSize.y = 70;
		tmpPos.x = 0;
		tmpPos.y = 0;
		addText(tmpPos, tmpSize, allSentences[sentenceID])
			.setTextAlign(TextAlign::CENTER)
			.setColor(colorise(s.j("colors").j("bg-rect").u("color"), s.j("colors").j("bg-rect").u("alpha")))
			.setZ(1);
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
void SceneLoading::load() {
	ASceneMenu::load();
}

/**
 * @brief this is the update function (called every frames)
 *
 * @return true if the update is a success
 * @return false if there are an error in update
 */
bool	SceneLoading::update() {
	ASceneMenu::update();
	SceneManager::loadScene(SceneNames::MAIN_MENU);
	return true;
}

/**
 * @brief this is the draw function (called every frames)
 *
 * @return true if the draw is a success
 * @return false if there are an error in draw
 */
bool SceneLoading::draw() {
	bool ret = true;
	ret = ASceneMenu::draw();
	return ret & true;
}
