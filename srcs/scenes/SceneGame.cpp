#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "SceneGame.hpp"
#include "bomberman.hpp"
#include "FileUtils.hpp"

#include "Player.hpp"
#include "Wall.hpp"
#include "Crispy.hpp"
#include "Flag.hpp"
#include "End.hpp"

#include "EnemyBasic.hpp"
#include "EnemyFollow.hpp"
#include "EnemyWithEye.hpp"

#include "SceneManager.hpp"

// -- Static members initialisation --------------------------------------------

std::map<std::string, SceneGame::Entity> SceneGame::_entitiesCall = {
	{"player", {EntityType::PLAYER, [](SceneGame &game) -> AEntity* {return new Player(game);}}},
	{"bomb", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Bomb(game);}}},
	{"wall", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Wall(game);}}},
	{"block", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Wall(game, Block::BLOCK);}}},
	{"crispy", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Crispy(game);}}},
	{"flag", {EntityType::BOARD_FLAG, [](SceneGame &game) -> AEntity* {return new Flag(game);}}},
	{"end", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new End(game);}}},
	{"safe", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {(void)game; return nullptr;}}},
	{"empty", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {(void)game; return nullptr;}}},
	{"enemyBasic", {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyBasic(game);}}},
	{"enemyWithEye", {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyWithEye(game);}}},
	{"enemyFollow", {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyFollow(game);}}},
};

// -- Constructors -------------------------------------------------------------

SceneGame::SceneGame(Gui * gui, float const &dtTime)
: ASceneMenu(gui, dtTime)
{
	player = nullptr;
	enemies = std::vector<AEnemy *>();
	flags = 0;
	size = {0, 0};
	level = NO_LEVEL;
	state = GameState::PLAY;
	time = std::chrono::milliseconds(0);
}

SceneGame::~SceneGame() {
	for (auto &&box : board) {
		for (auto &&row : box) {
			for (auto &&element : row) {
				delete element;
			}
		}
	}
	if (player != nullptr) {
		// TODO(ebaudet): save player if state is not GameOver.
		delete player;
	}
	auto enemy = enemies.begin();
	while (enemy != enemies.end()) {
		delete *enemy;
		// deleting an enemy also erase it from the list of enemies
		enemy = enemies.begin();
		if (enemy != enemies.end())
			enemy++;
	}

	for (auto it = _mapsList.begin(); it != _mapsList.end(); it++) {
		delete *it;
	}
	_mapsList.clear();
}

SceneGame::SceneGame(SceneGame const &src)
: ASceneMenu(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

SceneGame &SceneGame::operator=(SceneGame const &rhs) {
	if ( this != &rhs ) {
		logWarn("SceneGame object copied");
		board = rhs.board;
		player = rhs.player;
		enemies = rhs.enemies;
		flags = rhs.flags;
		size = rhs.size;
		level = rhs.level;
		state = rhs.state;
		time = rhs.time;
	}
	return *this;
}

std::ostream &	operator<<(std::ostream & os, const SceneGame& myClass) {
	os << myClass.print();
	return (os);
}

// -- Methods ------------------------------------------------------------------

/**
 * print params.
 */
std::string		SceneGame::print() const {
	std::string		str;
	str = "SceneGame info: [" + std::to_string(size.x) + ", "
	+ std::to_string(size.y) + "]";

	return str;
}

/**
 * init game method.
 */
bool			SceneGame::init() {
	_gui->enableCursor(false);
	int32_t i = 0;
	while (_initJsonLevel(i)) {
		if (i >= 100000) {  // max level
			break;
		}
		i++;
	}

	return true;
}

/**
 * @brief clear entity at position pos from board.
 *
 * @param entity
 * @param pos
 * @return true if cleared
 * @return false if not found
 */
bool			SceneGame::clearFromBoard(AEntity *entity, glm::vec2 pos) {
	std::vector<AEntity *> &box = board[pos.x][pos.y];
	std::vector<AEntity *>::iterator find = std::find(box.begin(), box.end(), entity);
	if (find == box.end())
		return false;
	box.erase(find);
	return true;
}

/**
 * @brief Check if the given pos is in the board.
 *
 * @param pos
 * @return true
 * @return false
 */
bool	SceneGame::positionInGame(glm::vec2 pos) {
	if (pos.x < 0 || pos.x > (size.x - 1) || pos.y < 0 || pos.y > (size.y - 1))
		return false;
	return true;
}

// -- AScene Methods -----------------------------------------------------------

/**
 * @brief update is called each frame.
 *
 * @return true
 * @return false
 */
bool	SceneGame::update() {
	if (level == NO_LEVEL)
		return true;

	if (Inputs::getKeyUp(InputType::CANCEL))
		state = GameState::PAUSE;

	if (state == GameState::PAUSE) {
		SceneManager::loadScene(SceneNames::PAUSE);
		return true;
	}
	else if (state == GameState::WIN) {
		SceneManager::loadScene(SceneNames::VICTORY);
		return true;
	}
	else if (state == GameState::GAME_OVER) {
		// clear game infos.
		player->resetParams();
		SceneManager::loadScene(SceneNames::GAME_OVER);
		return true;
	}

	for (auto &&board_it0 : board) {
		for (auto &&board_it1 : board_it0) {
			for (AEntity *board_it2 : board_it1) {
				if (!board_it2->update(_dtTime))
					return false;
			}
		}
	}
	for (auto &&enemy : enemies) {
		if (!enemy->update(_dtTime))
			return false;
	}
	player->update(_dtTime);

	_updateGameInfos();

	return postUpdate();
}

/**
 * @brief postUpdate is called each frame, after update.
 *
 * @return true
 * @return false
 */
bool	SceneGame::postUpdate() {
	player->postUpdate();
	auto enemy = enemies.begin();
	while (enemy != enemies.end()) {
		if (!(*enemy)->postUpdate()) {
			enemy = enemies.begin();
			continue;
		}
		enemy++;
	}

	for (auto &&board_it0 : board) {
		for (auto &&board_it1 : board_it0) {
			std::vector<AEntity *>::iterator it = board_it1.begin();
			AEntity * copy;
			while (it != board_it1.end()) {
				copy = *it;
				if (!(*it)->postUpdate()) {
					it = board_it1.begin();
					continue;
				}
				if (it == board_it1.end())
					continue;
				if (copy == *it)
					it++;
			}
		}
	}

	return true;
}

/**
 * @brief draw is called each frame to draw the Game Scene.
 *
 * @return true
 * @return false
 */
bool	SceneGame::draw() {
	// use cubeShader, set uniform and activate textures
	glm::mat4	view = _gui->cam->getViewMatrix();
	_gui->cubeShader->use();
	_gui->cubeShader->setMat4("view", view);
	_gui->cubeShader->setVec3("viewPos", _gui->cam->pos);
	glBindVertexArray(_gui->cubeShVao);
	_gui->textureManager->activateTextures();
	_gui->cubeShader->setInt("blockId", 0);
	_gui->cubeShader->unuse();

	for (auto &&board_it0 : board) {
		for (auto &&board_it1 : board_it0) {
			for (AEntity *board_it2 : board_it1) {
				if (!board_it2->draw(*_gui))
					return false;
			}
		}
	}
	for (auto &&enemy : enemies) {
		if (!enemy->draw(*_gui))
			return false;
	}
	player->draw(*_gui);

	// draw board
	_gui->drawCube(Block::FLOOR, {0.0f, -0.3f, size.y - 1.0f}, {size.x, 0.3f, size.y});

	// release cubeShader and textures
	_gui->cubeShader->use();
	_gui->textureManager->disableTextures();
	_gui->cubeShader->unuse();

	ASceneMenu::draw();

	// draw skybox
	_gui->drawSkybox(view);

	return true;
}

/**
 * @brief called when the scene is loaded
 */
void SceneGame::load() {
	if (state == GameState::PAUSE
	|| state == GameState::WIN
	|| state == GameState::GAME_OVER) {
		state = GameState::PLAY;
	}
	_gui->enableCursor(false);
}
/**
 * @brief called when the scene is unloaded
 */
void SceneGame::unload() {
}

/**
 * @brief load a level by ID
 *
 * @param levelId the level ID
 * @return true if the level loading is a success
 * @return false if the level loading failed
 */
bool SceneGame::loadLevel(int32_t levelId) {
	logInfo("load level " << levelId);
	if (_unloadLevel() == false) {
		level = NO_LEVEL;
		return false;
	}
	bool result = _loadLevel(levelId);

	_gui->cam->pos = {size.x / 2, 25.0f, 2 * size.y};
	_gui->cam->lookAt(glm::vec3(
		size.x / 2, 1.0f,
		size.y / 1.61803398875f
	));

	player->init();

	return result;
}

// -- Private Methods ----------------------------------------------------------

bool	SceneGame::_initJsonLevel(int32_t levelId) {
	// level$(levelID)
	std::string		levelName = "level" + std::to_string(levelId);
	// $(mapsPath)/$(levelName).json
	std::string		filename = s.s("mapsPath") + "/" + levelName + ".json";
	if (file::isFile(filename) == false) {
		return false;  // file does not exist
	}

	SettingsJson *	lvl = new SettingsJson();

	lvl->name(levelName).description("Level map");
	lvl->add<std::string>(levelName + "Filename", filename);

	// File json definition:
	lvl->add<std::string>("name");
	lvl->add<std::string>("img", "bomberman-assets/img/icon_level1");
	lvl->add<uint64_t>("height", 0).setMin(0).setMax(100);
	lvl->add<uint64_t>("width", 0).setMin(0).setMax(100);
	lvl->add<int64_t>("time", 0).setMin(-1).setMax(86400);

	// foreach empty zone, chance to create a wall
	lvl->add<uint64_t>("wallGenPercent", 40).setMin(0).setMax(100);

	lvl->add<SettingsJson>("objects");
		lvl->j("objects").add<std::string>("empty", " ");
		// unique player on game.
		lvl->j("objects").add<std::string>("player", "p");
		// destructing element dropped by the player.
		lvl->j("objects").add<std::string>("bomb", "x");
		// indestructible element outside the board
		lvl->j("objects").add<std::string>("wall", "w");
		// indestructible element of the board
		lvl->j("objects").add<std::string>("block", "b");
		// destructable element, who can give bonuses randomly
		lvl->j("objects").add<std::string>("crispy", "c");
		// flag to get end
		lvl->j("objects").add<std::string>("flag", "f");
		// end of level when all flag
		lvl->j("objects").add<std::string>("end", "e");
		// no spawn zone
		lvl->j("objects").add<std::string>("safe", "_");
		/* enemies */
		lvl->j("objects").add<std::string>("enemyBasic", "0");
		lvl->j("objects").add<std::string>("enemyWithEye", "1");
		lvl->j("objects").add<std::string>("enemyFollow", "2");

	SettingsJson * mapPattern = new SettingsJson();
	mapPattern->add<std::string>("line", "");
	lvl->addList<SettingsJson>("map", mapPattern);

	lvl->add<SettingsJson>("bonus");
		for (auto &&pair : Bonus::bonus) {
			lvl->j("bonus").add<SettingsJson>(pair.first);
			lvl->j("bonus").j(pair.first).add<int64_t>("chance", 0).setMin(0).setMax(25);
			lvl->j("bonus").j(pair.first).add<int64_t>("nb", -1).setMin(-1).setMax(100);
		}
	try {
		if (lvl->loadFile(filename) == false) {
			// warning when loading settings
			return true;
		}
	} catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
		return false;
	}

	_mapsList.push_back(lvl);

	return true;
}

bool	SceneGame::_unloadLevel() {
	if (level == NO_LEVEL)
		return true;

	for (auto &&box : board) {
		for (auto &&row : box) {
			for (auto &&element : row) {
				delete element;
			}
		}
	}
	board.clear();
	auto enemy = enemies.begin();
	while (enemy != enemies.end()) {
		delete *enemy;
		// deleting an enemy also erase it from the list of enemies
		enemy = enemies.begin();
		if (enemy != enemies.end())
			enemy++;
	}
	enemies.clear();

	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		delete *it;
	}
	_buttons.clear();

	level = NO_LEVEL;
	return true;
}

/**
 * @brief Load Level method. Throw a SceneException if the level is incomplete.
 *
 * @param levelId
 * @return true
 * @return false
 */
bool	SceneGame::_loadLevel(int32_t levelId) {
	if (levelId == NO_LEVEL)
		return true;
	if (levelId > (int32_t)_mapsList.size()) {
		logErr("unable to load level " << levelId << ": doesn't exist");
		return false;
	}

	level = levelId;  // save new level ID
	SettingsJson & lvl = *(_mapsList[level]);

	// Getting json info
	size = {lvl.u("width"), lvl.u("height")};
	board = std::vector< std::vector<std::vector<AEntity*>> >(size.x,
			std::vector< std::vector<AEntity*> >(size.y,
			std::vector< AEntity* >()));

	if (lvl.lj("map").list.size() != size.y)
		throw SceneException("Map height error");

	time = std::chrono::seconds(lvl.i("time"));

	flags = 0;
	AEntity *entity;
	// Get map informations
	for (uint32_t j = 0; j < size.y; j++) {
		std::string line = lvl.lj("map").list[j]->s("line");
		if (line.length() != size.x)
			throw SceneException(("Map width error on line "+std::to_string(j)).c_str());
		for (uint32_t i = 0; i < size.x; i++) {
			for (auto &&entitYCall : _entitiesCall) {
				if (line[i] == lvl.j("objects").s(entitYCall.first)[0]) {
					// if it's empty, generate crispy wall with a certain probability
					if (entitYCall.first == "empty")
						entity = Crispy::generateCrispy(*this, lvl.u("wallGenPercent"));
					else
						entity = _entitiesCall[entitYCall.first].entity(*this);
					if (entity == nullptr)
						continue;
					switch (_entitiesCall[entitYCall.first].entityType) {
					case EntityType::PLAYER:
						if (player == nullptr)
							player = reinterpret_cast<Player *>(entity);
						player->setPosition({i, 0, j});
						break;
					case EntityType::BOARD_FLAG:
						flags++;
						board[i][j].push_back(entity);
						break;
					case EntityType::BOARD:
						board[i][j].push_back(entity);
						break;
					case EntityType::ENEMY:
						enemies.push_back(reinterpret_cast<AEnemy *>(entity));
						enemies.back()->setPosition({i, 0, j});
						break;
					}
				}
			}
		}
	}

	if (player == nullptr)
		throw SceneException("No player on this level.");

	bool	end = false;
	for (uint32_t j = 0; j < size.y; j++) {
		for (uint32_t i = 0; i < size.x; i++) {
			for(auto entity : board[i][j]) {
				if (entity->type == Type::END)
					end = true;
			}
		}
	}
	if (!end)
		throw SceneException("No end on this level.");

	_initBonus();

	// set camera
	_gui->cam->lookAt(glm::vec3(size.x / 2 + 0.5f, 1.0f, size.y * 0.7f));

	return true;
}

/**
 * @brief Update game informations
 */
void			SceneGame::_updateGameInfos() {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		delete *it;
	}
	_buttons.clear();

	glm::vec2	winSz = _gui->gameInfo.windowSize;
	glm::vec2	tmpPos;
	glm::vec2	tmpSize;
	uint32_t	padding = 5;
	float		menuWidth = winSz.x / 2;
	float		menuHeight = menuWidth / 8;

	try {
		tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;
		tmpSize.x = menuWidth;
		tmpSize.y = menuHeight;
		tmpSize = {32, 32};

		tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/012-bonus_life.png").getSize().x;
		tmpPos.x += addText(tmpPos, VOID_SIZE, std::to_string(player->lives)).setTextAlign(TextAlign::LEFT).getSize().x;
		tmpPos.x += padding;
		tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/015-bonus_speed.png").getSize().x;
		std::string	speed = std::to_string(player->speed);
		speed = speed.substr(0, speed.find("."));
		tmpPos.x += addText(tmpPos, VOID_SIZE, speed).setTextAlign(TextAlign::LEFT).getSize().x;
		tmpPos.x += padding;
		tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/013-bonus_bomb.png").getSize().x;
		tmpPos.x += addText(tmpPos, VOID_SIZE, std::to_string(player->totalBombs)).setTextAlign(TextAlign::LEFT)
					.getSize().x;
		tmpPos.x += padding;
		tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/014-bonus_flame.png").getSize().x;
		tmpPos.x += addText(tmpPos, VOID_SIZE, std::to_string(player->bombProgation)).setTextAlign(TextAlign::LEFT)
					.getSize().x;

		if (player->passFire) {
			tmpPos.x += padding;
			tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/019-bonus_flampass.png")
						.getSize().x;
		}
		if (player->passWall) {
			tmpPos.x += padding;
			tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/016-bonus_wallpass.png")
						.getSize().x;
		}
		if (player->detonator) {
			tmpPos.x += padding;
			tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/017-bonus_detonator.png")
						.getSize().x;
		}
		if (player->passBomb) {
			tmpPos.x += padding;
			tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/018-bonus_bombpass.png")
						.getSize().x;
		}
		if (player->invulnerable > 0) {
			tmpPos.x += padding;
			tmpPos.x += addImage(tmpPos, tmpSize, "bomberman-assets/textures/bonus/020-bonus_shield.png")
						.getSize().x;
			std::string	invulnerable = std::to_string(player->invulnerable);
			invulnerable = invulnerable.substr(0, invulnerable.find(".")+2);
			tmpPos.x += addText(tmpPos, VOID_SIZE, invulnerable).setTextAlign(TextAlign::LEFT).getSize().x;
		}
	} catch (ABaseUI::UIException const & e) {
		logErr(e.what());
	}
}

/**
 * @brief Init the bonus member according to the description in level.json
 *
 * @return true if success
 * @return false if error.
 */
bool			SceneGame::_initBonus() {
	try {
		if (bonus.size())
			bonus.erase(bonus.begin(), bonus.end());
		SettingsJson	&lvl = getSettingsLevel();
		for (auto &&pair : Bonus::bonus) {
			bonus.insert({
				pair.first,
				{
					lvl.j("bonus").j(pair.first).i("chance"),
					lvl.j("bonus").j(pair.first).i("nb")
				}
			});
		}
	} catch (SceneGameException &e) {
		logErr(e.what());
		return false;
	}
	return true;
}

// -- getter -------------------------------------------------------------------

uint32_t	SceneGame::getNbLevel() const { return _mapsList.size(); }
std::string	SceneGame::getLevelName(int32_t levelId) const {
	if (levelId == NO_LEVEL)
		return "NO_LEVEL";
	return _mapsList[levelId]->s("name");
}
std::string	SceneGame::getLevelImg(int32_t levelId) const {
	if (levelId == NO_LEVEL) {
		logErr("can't get image for level 'NO_LEVEL'");
		return "";
	}
	return _mapsList[levelId]->s("img");
}

/**
 * @brief Return JSON Settings of level.
 *
 * @return SettingsJson& JSON Settings of level
 * @throw SceneGameException if error
 */
SettingsJson	&SceneGame::getSettingsLevel() const {
	if (level == NO_LEVEL)
		throw SceneGameException("no level set");
	if (level > (int32_t)_mapsList.size())
		throw SceneGameException(("unable to load level " + std::to_string(level) + ": doesn't exist").c_str());
	return *(_mapsList[level]);
}

// -- Exceptions errors --------------------------------------------------------

SceneGame::SceneGameException::SceneGameException()
: std::runtime_error("SceneGame Exception") {}

SceneGame::SceneGameException::SceneGameException(const char* whatArg)
: std::runtime_error(std::string(std::string("SceneGameError: ") + whatArg).c_str()) {}
