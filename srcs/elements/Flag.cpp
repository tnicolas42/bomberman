#include "Flag.hpp"
#include "SceneGame.hpp"
#include "AudioManager.hpp"

// -- Constructors -------------------------------------------------------------

/**
 * @brief Construct a new Flag:: Flag object
 *
 * @param game A reference to the SceneGame master object
 */
Flag::Flag(SceneGame &game) : AObject(game) {
	type = Type::FLAG;
	name = FLAG_STR;
	destructible = true;
	blockPropagation = true;
	AudioManager::loadSound(FLAG_DESTROYED_SOUND);
	_soundOfDeath = FLAG_DESTROYED_SOUND;
}

/**
 * @brief Destroy the Flag:: Flag object
 */
Flag::~Flag() {
	if (game.clearFromBoard(this, {position.x, position.z})) {
		game.flags--;
	}
}

/**
 * @brief Construct a new Flag:: Flag object
 *
 * @param src The object to do the copy
 */
Flag::Flag(Flag const &src) : AObject(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

/**
 * @brief Copy this object
 *
 * @param rhs The opjet to copy
 * @return Flag& A reference to the copied object
 */
Flag &Flag::operator=(Flag const &rhs) {
	if ( this != &rhs ) {
		AObject::operator=(rhs);
	}
	return *this;
}

// -- Methods ------------------------------------------------------------------

/**
 * @brief update is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	Flag::update() {
	return true;
}

/**
 * @brief postUpdate is called each frame. After update.
 *
 * @return true if success
 * @return false if failure
 */
bool	Flag::postUpdate() {
	if (active == false || alive == false)
		delete this;
	return true;
}

/**
 * @brief draw is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	Flag::draw(Gui &gui) {
	gui.drawCube(Block::FLAG, getPos());
	return true;
}

// -- Exceptions errors --------------------------------------------------------

Flag::FlagException::FlagException()
: std::runtime_error("Flag Exception") {}

Flag::FlagException::FlagException(const char* whatArg)
: std::runtime_error(std::string(std::string("FlagError: ") + whatArg).c_str()) {}
