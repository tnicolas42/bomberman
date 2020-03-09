#include <unistd.h>
#include <chrono>

#include "SceneManager.hpp"
#include "bomberman.hpp"
#include "SceneGame.hpp"
#include "SceneMenu.hpp"

SceneManager::SceneManager()
: _scene(nullptr),
  _gameInfo(),
  _gui(nullptr),
  _dtTime(0.0f) {}

SceneManager::~SceneManager() {
	delete _scene;
	delete _gui;
}

SceneManager::SceneManager(SceneManager const & src) {
	*this = src;
}

SceneManager & SceneManager::operator=(SceneManager const & rhs) {
	if (this != &rhs) {
		logWarn("SceneManager object copied");
		_scene = rhs._scene;
		_gameInfo = rhs._gameInfo;
		_gui = rhs._gui;
		_dtTime = rhs._dtTime;
	}
	return *this;
}

/**
 * @brief init the SceneManager (load the first scene& load gui)
 *
 * @return true if success
 * @return false if failure
 */
bool SceneManager::init() {
	// check double call of init function
	if (_scene != nullptr) {
		logWarn("SceneManager init is already called");
	}

	// create and init first gui
	_gui = new Gui(_gameInfo);
	if (!_gui->init()) {
		return false;
	}

	// test Scene Game
	SceneGame game = SceneGame(_gui, _dtTime);

	try {
		game.init();
	} catch (std::exception &e) {
		logErr("Error : " << e.what());
	}

	// create and init first scene
	// TODO(tnicolas42) create the scene loader (factory ?)
	// _scene = new SceneGame(_gui, _dtTime);
	_scene = new SceneMenu(_gui, _dtTime);
	if (_scene->init() == false) {
		logErr("failed to init scene");
		return false;
	}
	return true;
}

/**
 * @brief this is the main game loop to update & draw everythings
 *
 * @return true if success
 * @return false if failure
 */
bool SceneManager::run() {
	float						fps = 1000 / s.j("screen").u("fps");
	std::chrono::milliseconds	lastLoopMs = getMs();

	while (true) {
		if (_scene == nullptr) {
			logWarn("scene object is null");
		}
		else {
			// update dtTime
			_dtTime = (getMs().count() - lastLoopMs.count()) / 1000.0;
			lastLoopMs = getMs();

			// get inputs
			Inputs::update();
			_gui->updateInput(_dtTime);

			// update the scene
			if (!_scene->update()) {
				return false;
			}

			// draw the gui
			_gui->preDraw();
			if (!_scene->draw()) {
				return false;
			}
			_gui->postDraw();

			// quit if it's the end of the game
			if (_gameInfo.quit) {
				logInfo("exit game")
				break;
			}
		}

		// fps
		std::chrono::milliseconds loopDuration = getMs() - lastLoopMs;
		if (loopDuration.count() > fps) {
			#if DEBUG_FPS_LOW == true
				if (!firstLoop)
					logDebug("update loop slow -> " << loopDuration.count() << "ms / " << fps << "ms (" << FPS << "fps)");
			#endif
		}
		else {
			usleep((fps - loopDuration.count()) * 1000);
		}
		#if DEBUG_FPS_LOW == true
			firstLoop = false;
		#endif
	}
	return true;
}

/* exception */
SceneManager::SceneManagerException::SceneManagerException()
: std::runtime_error("SceneManager Exception") {}

SceneManager::SceneManagerException::SceneManagerException(const char* whatArg)
: std::runtime_error(std::string(std::string("SceneManagerException: ") + whatArg).c_str()) {}
