#include "Gui.hpp"
#include "Logging.hpp"
#include "Inputs.hpp"
#include "SceneManager.hpp"
#include "Material.hpp"
#include "ABaseUI.hpp"
#include "BoxCollider.hpp"

// -- Gui ---------------------------------------------------------------
/**
 * @brief Construct a new Gui:: Gui object
 *
 * @param gameInfo The gameinfo object
 */
Gui::Gui(GameInfo &gameInfo)
: gameInfo(gameInfo),
  textureManager(nullptr),
  cubeShader(nullptr),
  cam(nullptr),
  _win(nullptr),
  _event(new SDL_Event()),
  _context(0),
  _skybox(nullptr),
  _exitMenuDisabled(false) {}

/**
 * @brief Destroy the Gui:: Gui object
 */
Gui::~Gui() {
	logDebug("exit SDL");

	// free vao / vbo
	cubeShader->use();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &cubeShVbo);
	glDeleteVertexArrays(1, &cubeShVao);
	cubeShader->unuse();

	delete _event;
	delete textureManager;
	delete cubeShader;
	delete cam;
	delete _skybox;

	ABaseUI::destroy();
	BoxCollider::destroy();

	// properly quit sdl
	SDL_GL_DeleteContext(_context);
	try {
		SDL_DestroyWindow(_win);
		SDL_Quit();
	}
	catch (std::exception & e) {
		logWarn("failed to exit proprely SDL window");
	}
}

/**
 * @brief Construct a new Gui:: Gui object
 *
 * @param src The object to do the copy
 */
Gui::Gui(Gui const &src)
: gameInfo(src.gameInfo) {
	*this = src;
}

/**
 * @brief Copy this object
 *
 * @param rhs The object to copy
 * @return Gui& A reference to the copied object
 */
Gui &Gui::operator=(Gui const &rhs) {
	if (this != &rhs) {
		logErr("The copy operator should not be called")
	}
	return *this;
}

/**
 * @brief Called each frame before others updates functions
 *
 * @param dtTime The delta time since last call
 */
void Gui::preUpdate(float const dtTime) {
	(void)dtTime;
	if (Inputs::isConfiguring())
		return;

	/* open cheat code */
	if (Inputs::getKeyUp(InputType::CHEAT_CODE)) {
		SceneManager::openCheatCode(true);
	}

	cam->mouseSensitivity = s.d("mouse_sensitivity");
}

/**
 * @brief Called each frame after others updates functions
 *
 * @param dtTime The delta time since last call
 */
void Gui::postUpdate(float const dtTime) {
	(void)dtTime;
	/* quit if needed */
	if (Inputs::shouldQuit()
	|| (!Inputs::isConfiguring() && Inputs::getKeyUp(InputType::CANCEL) && _exitMenuDisabled == false))
	{
		bool cheatCodeState = SceneManager::isCheatCodeOpen();
		if (cheatCodeState) {
			SceneManager::openCheatCode(false);  // force quit cheat code
		}
		if (Inputs::shouldQuit() || cheatCodeState == false) {
			#if ASK_BEFORE_QUIT
				if (SceneManager::getSceneName() != SceneNames::EXIT) {
					SceneManager::loadScene(SceneNames::EXIT);
				}
			#else
				SceneManager::quit();
			#endif
		}
	}
	_exitMenuDisabled = false;
}

// -- init ---------------------------------------------------------------------
/**
 * @brief init the gui object
 *
 * @return true if the gui was init successfully
 * @return false if there is an error in init
 */
bool	Gui::init() {
	logDebug("create gui");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		logErr("while loading SDL: " << SDL_GetError());
		SDL_Quit();
		return false;
	}

	if (!_protect_resolution()) {
		return false;
	}

	// create window and init opengl settings
	if (!_initOpengl()) {
		return false;
	}

	// init shaders attributes
	if (!_initShaders()) {
		return false;
	}

	/* init UI interface */
	try {
		gameInfo.savedWindowSize.x = s.j("graphics").i("width");
		gameInfo.savedWindowSize.y = s.j("graphics").i("height");
		gameInfo.isSavedFullscreen = s.j("graphics").b("fullscreen");
		ABaseUI::init(gameInfo.windowSize, s.j("fonts").j("base").s("file"), s.j("fonts").j("base").u("size"));
		ABaseUI::loadFont("title", s.j("fonts").j("base").s("file"), s.j("fonts").j("base").u("size") * 2.5);
		ABaseUI::loadFont("cheatcode", s.j("fonts").j("cheatcode").s("file"), s.j("fonts").j("cheatcode").u("size") * 3);
		ABaseUI::loadFont("text", s.j("fonts").j("text").s("file"), s.j("fonts").j("text").u("size") * 2);
		ABaseUI::setHelpToogleInput(InputType::SHOW_HELP);
		ABaseUI::showHelp(DEBUG_SHOW_HELP);
	}
	catch (ABaseUI::UIException const & e) {
		logErr(e.what());
		return false;
	}

	if (BoxCollider::init(this) == false) {
		return false;
	}

	return true;
}


// -- _initOpengl --------------------------------------------------------------
/**
 * @brief init openGL (called in Gui::init)
 *
 * @return true if success
 * @return false if error
 */
bool	Gui::_initOpengl() {
	// opengl version 4.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// create window
	_win = SDL_CreateWindow(gameInfo.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		gameInfo.windowSize.x, gameInfo.windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (_win == nullptr) {
		logErr("while loading OpenGL: " << SDL_GetError());
		return false;
	}
	if (s.j("graphics").b("fullscreen")) {
		if (SDL_SetWindowFullscreen(_win, SDL_WINDOW_FULLSCREEN) != 0) {
			logErr("failed to set window fullscreen.")
		}
	}

	// create opengl context
	_context = SDL_GL_CreateContext(_win);
	if (_context == 0) {
		logErr("while loading OpenGL: " << SDL_GetError());
		return false;
	}
	// init glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		logErr("while loading OpenGL: failed to init glad");
		return false;
	}

	glEnable(GL_MULTISAMPLE);  // anti aliasing
	glEnable(GL_CULL_FACE);  // face culling
	glEnable(GL_BLEND);  // enable blending (used in textRender)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	return true;
}

// -- _initShaders -------------------------------------------------------------
/**
 * @brief create opengl shader stuffs here (buffers, camera, ...)
 *
 * @return true if success
 * @return false if error
 */
bool	Gui::_initShaders() {
	// -- create shader -`------------------------------------------------------
	cubeShader = new Shader("shaders/cube_vs.glsl",
		"shaders/cube_fs.glsl",
		"shaders/cube_gs.glsl");
	cubeShader->use();

	// -- textureManager -------------------------------------------------------
	textureManager = new TextureManager();
	textureManager->setUniform(*cubeShader);

	// -- vao vbo --------------------------------------------------------------
	// create and bind vao and vbo
	glGenVertexArrays(1, &cubeShVao);
	glBindVertexArray(cubeShVao);
	glGenBuffers(1, &cubeShVbo);
	glBindBuffer(GL_ARRAY_BUFFER, cubeShVbo);

	// fill buffer
	glBufferData(GL_ARRAY_BUFFER, _cubeFaces.size() * sizeof(float),
		&_cubeFaces[0], GL_STATIC_DRAW);

	// bottom left corner face pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, C_VAO_WIDTH * sizeof(float),
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// face id
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, C_VAO_WIDTH * sizeof(float),
		reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind vao / vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// -- camera ---------------------------------------------------------------
	float ratio = static_cast<float>(gameInfo.windowSize.x) / gameInfo.windowSize.y;
	cam = new Camera(ratio, {0.0f, 25.0f, 0.0f});

	// -- set default uniforms -------------------------------------------------
	// projection
	cubeShader->setMat4("projection", cam->getProjection());

	// cube material
	Material material;
	cubeShader->setVec3("material.specular", material.specular);
	cubeShader->setFloat("material.shininess", material.shininess);

	// direction light
	cubeShader->setVec3("dirLight.direction", -0.2f, -0.8f, 0.6f);
	cubeShader->setVec3("dirLight.ambient", 0.4f, 0.4f, 0.4f);
	cubeShader->setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
	cubeShader->setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);

	// point light
	cubeShader->setFloat("pointLight.constant", 1.0f);
	cubeShader->setFloat("pointLight.linear", 0.09f);
	cubeShader->setFloat("pointLight.quadratic", 0.032f);

	cubeShader->setVec3("pointLight.ambient", 0.4f, 0.4f, 0.4f);
	cubeShader->setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
	cubeShader->setVec3("pointLight.specular", 0.1f, 0.1f, 0.1f);
	// disabled for now
	cubeShader->setBool("pointLight.enabled", false);

	// disable texture transparency for now
	cubeShader->setBool("enableTransparency", true);

	// disable fog for now
	cubeShader->setBool("fog.enabled", false);

	cubeShader->unuse();

	// -- skybox ---------------------------------------------------------------
	_skybox = new Skybox;
	_skybox->getShader().use();
	_skybox->getShader().setMat4("projection", cam->getProjection());
	_skybox->getShader().unuse();

	return true;
}

/**
 * @brief Protect the resolution read in the config file and update it if required.
 *
 * @return false if the screen is too small to display the game correctly.
 */
bool	Gui::_protect_resolution() {
	bool			resolution_corrected = false;
	SDL_DisplayMode dm;
	int64_t			&width = s.j("graphics").i("width");
	int64_t			&height = s.j("graphics").i("height");

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		logErr("SDL_GetDesktopDisplayMode failed: %s" << SDL_GetError());
	}
	if (dm.w < Gui::_min_width || dm.h < Gui::_min_height) {
		logWarn("Screen too small.");
		return false;
	}
	gameInfo.maxWindowSize.x = dm.w;
	gameInfo.maxWindowSize.y = dm.h;
	if (s.j("graphics").b("fitToScreen")) {
		s.j("graphics").i("width") = gameInfo.maxWindowSize.x;
		s.j("graphics").i("height") = gameInfo.maxWindowSize.y;
		resolution_corrected = true;
	}
	if (dm.w < width) {
		width = dm.w;
		resolution_corrected = true;
	}
	if (dm.h < height) {
		height = dm.h;
		resolution_corrected = true;
	}
	if (static_cast<double>(width) / static_cast<double>(height) < 4.0 / 3.0) {
		logWarn("ratio too small");
		width = height * 4.0 / 3.0;
		resolution_corrected = true;
	}
	if (static_cast<double>(width) / static_cast<double>(height) > 16.0 / 9.0) {
		logWarn("ratio too big");
		height = width * 9.0 / 16.0;
		resolution_corrected = true;
	}
	logDebug("Screen resolution: " << width << "x" << height);
	if (resolution_corrected) {
		gameInfo.windowSize.x = width;
		gameInfo.windowSize.y = height;
		s.saveToFile(homeDir+SETTINGS_FILE);
	}
	return true;
}

// -- enableCursor -------------------------------------------------------------
/**
 * @brief enable or disable the cursor visibility
 *
 * @param enable enable boolean
 */
void Gui::enableCursor(bool enable) {
	if (enable) {
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	else {
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}

// -- drawCube -----------------------------------------------------------------
/**
 * @brief Draw a cube on the screen
 *
 * @param typeBlock The type of cube
 * @param pos The position
 * @param scale The scale
 */
void	Gui::drawCube(Block::Enum typeBlock, glm::vec3 pos, glm::vec3 scale) {
	glm::mat4	model(1.0);
	cubeShader->use();

	// set uniform and activate textures
	glm::mat4	view = cam->getViewMatrix();
	cubeShader->setMat4("view", view);
	cubeShader->setVec3("viewPos", cam->pos);
	glBindVertexArray(cubeShVao);
	textureManager->activateTextures();

	// set block size
	cubeShader->setVec3("blockSize", scale);
	// set block type
	cubeShader->setInt("blockId", typeBlock);
	// set block pos
	model = glm::translate(glm::mat4(1.0), pos);
	cubeShader->setMat4("model", model);
	glDrawArrays(GL_POINTS, 0, C_NB_FACES);  // draw

	textureManager->disableTextures();
	cubeShader->unuse();
}

// -- change settings ----------------------------------------------------------
/**
 * Update the fullscreen mode from its settings.
 */
void	Gui::updateFullscreen() {
	if (s.j("graphics").b("fullscreen")) {
		SDL_SetWindowFullscreen(_win, SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(_win, 0);
		SDL_SetWindowPosition(_win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

/**
 * Update the dimension of the window from its settings.
 */
void	Gui::udpateDimension() {
	gameInfo.windowSize.x = s.j("graphics").i("width");
	gameInfo.windowSize.y = s.j("graphics").i("height");
	SDL_SetWindowSize(_win, gameInfo.windowSize.x, gameInfo.windowSize.y);
	ABaseUI::setWinSize({gameInfo.windowSize.x, gameInfo.windowSize.y});
	SDL_SetWindowPosition(_win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

// -- draw ---------------------------------------------------------------------
/**
 * @brief call this function to do stuff before drawing scene
 */
void Gui::preDraw() {
	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, gameInfo.windowSize.x, gameInfo.windowSize.y);
	glClearColor(0.2, 0.5, 0.95, 1.0);
}

/**
 * @brief call this function to do stuff after drawing scene
 */
void Gui::postDraw() {
	// swap buffer and check errors
	SDL_GL_SwapWindow(_win);
	checkError();
}

// -- _drawSkybox --------------------------------------------------------------
/**
 * @brief draw the skybox
 *
 * @param view the view matrix of the camera (Camera::getViewMatrix())
 */
void	Gui::drawSkybox(glm::mat4 &view) {
	CAMERA_MAT4	skyView = view;
	skyView[3][0] = 0;  // remove translation for the skybox
	skyView[3][1] = 0;
	skyView[3][2] = 0;
	_skybox->getShader().use();
	_skybox->getShader().setMat4("view", skyView);
	_skybox->getShader().unuse();
	_skybox->draw(0.5);
	_skybox->getShader().unuse();
}

/**
 * @brief In this frame, the update function cannot exit
 *
 * @param disable True to enable this functionality
 */
void	Gui::disableExitForThisFrame(bool disable) {
	_exitMenuDisabled = disable;
}

// -- statics const ------------------------------------------------------------
// cube faces
std::array<float, C_FACE_A_SIZE> const	Gui::_cubeFaces = {{
	// bot left corner,		faceId
	0, 0, 1,	0,
	1, 0, 1,	1,
	1, 0, 0,	2,
	0, 0, 0,	3,
	0, 1, 1,	4,
	0, 0, 0,	5,
}};

// -- GameInfo struct ----------------------------------------------------------
GameInfo::GameInfo() {
	title = "bomberman";
	windowSize = {
		s.j("graphics").i("width"),
		s.j("graphics").i("height")
	};
	quit = false;
}
