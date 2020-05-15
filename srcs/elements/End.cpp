#include "End.hpp"
#include "SceneGame.hpp"
#include "Player.hpp"
#include "AudioManager.hpp"

// -- Constructors -------------------------------------------------------------

/**
 * @brief Construct a new End:: End object
 *
 * @param game A reference to the SceneGame master object
 */
End::End(SceneGame &game) : AObject(game) {
	type = Type::END;
	name = END_STR;
	_texture = Block::END;
	AudioManager::loadSound(END_OPEN_SOUND);
	_open = false;
}

/**
 * @brief Destroy the End:: End object
 */
End::~End() {
}

/**
 * @brief Construct a new End:: End object
 *
 * @param src The object to do the copy
 */
End::End(End const &src) : AObject(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

/**
 * @brief Copy this object
 *
 * @param rhs The opjet to copy
 * @return End& A reference to the copied object
 */
End &End::operator=(End const &rhs) {
	if (this != &rhs) {
		AObject::operator=(rhs);
		_texture = rhs._texture;
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
bool	End::update() {
	if (game.flags <= 0) {
		if (game.enemiesToKill <= game.enemiesKilled) {
			if (!_open) {
				_texture = Block::END_OPEN;
				try {
					AudioManager::playSound(END_OPEN_SOUND);
				} catch(Sound::SoundException const & e) {
					logErr(e.what());
				}
				_open = true;
			}
			if (std::find(game.player->crossableTypes.begin(), game.player->crossableTypes.end(), Type::END)
			== game.player->crossableTypes.end())
			{
				game.player->crossableTypes.push_back(Type::END);
			}
			std::unordered_set<AEntity *> collisions = _getCollision();
			for (auto &&entity : collisions) {
				if (entity->type == Type::PLAYER) {
					game.state = GameState::WIN;
				}
			}
		}
	}
	return true;
}

/**
 * @brief draw is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	End::draw(Gui &gui) {
	gui.drawCube(_texture, getPos());
	return true;
}

// -- Private Methods ----------------------------------------------------------

/**
 * @brief get a list of entity in collision with the End.
 *
 * @return std::unordered_set<AEntity *> collisions
 */
std::unordered_set<AEntity *>	End::_getCollision() {
	std::unordered_set<AEntity *> collisions;

	if (game.player->hasCollision(position))
		collisions.insert(game.player);

	return collisions;
}

// -- Exceptions errors --------------------------------------------------------

End::EndException::EndException()
: std::runtime_error("End Exception") {}

End::EndException::EndException(const char* whatArg)
: std::runtime_error(std::string(std::string("EndError: ") + whatArg).c_str()) {}
