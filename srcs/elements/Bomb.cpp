#include "Bomb.hpp"
#include "Fire.hpp"
#include "SceneGame.hpp"
#include "Player.hpp"
#include "AudioManager.hpp"

// -- Constructors -------------------------------------------------------------

Bomb::Bomb(SceneGame &game) : AObject(game) {
	type = Type::BOMB;
	name = "Bomb";
	_countdown = 2.0f;
	_propagation = 3;
	blockPropagation = true;
	AudioManager::loadSound(BOMB_EXPLOSION_SOUND);
}

Bomb::~Bomb() {
	try {
		game.clearFromBoard(this, {position.x, position.z});
	}
	catch (AObject::AObjectException const & e) {
		// the object was never placed on the board
	}
}

Bomb::Bomb(Bomb const &src) : AObject(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

Bomb &Bomb::operator=(Bomb const &rhs) {
	if ( this != &rhs ) {
		AObject::operator=(rhs);
		_countdown = rhs._countdown;
		_propagation = rhs._propagation;
	}
	return *this;
}

// -- Getters / Setters --------------------------------------------------------

Bomb*	Bomb::setPropagation(const int propagation) {
	_propagation = propagation;
	return this;
}

// -- Methods ------------------------------------------------------------------

/**
 * @brief The bomb explode in the N.E.S.W. directions at _propagation distance.
 *
 * @param pos
 */
void	Bomb::explode(glm::vec2 const pos) {
	int						i;
	std::vector<AEntity *>	box;

	try {
		AudioManager::playSound(BOMB_EXPLOSION_SOUND);
	} catch(Sound::SoundException const & e) {
		logErr(e.what());
	}

	active = false;
	_propagationExplosion({pos.x, pos.y});
	// top
	i = 0;
	while (++i < _propagation) {
		if (!_propagationExplosion({pos.x, pos.y + i}))
			break;
	}
	// right
	i = 0;
	while (++i < _propagation) {
		if (!_propagationExplosion({pos.x + i, pos.y}))
			break;
	}
	// bottom
	i = 0;
	while (++i < _propagation) {
		if (!_propagationExplosion({pos.x, pos.y - i}))
			break;
	}
	// left
	i = 0;
	while (++i < _propagation) {
		if (!_propagationExplosion({pos.x - i, pos.y}))
			break;
	}

	Fire *fire = new Fire(game);
	game.board[pos.x][pos.y].push_back(fire);
	fire->init();
	game.player->addBomb();
	alive = false;
}

/**
 * @brief TakeDamage make explode the bomb.
 *
 * @param damage
 * @return true if damage taken
 * @return false if damage not taken
 */
bool	Bomb::takeDamage(const int damage) {
	if (!active || damage <= 0)
		return false;

	_countdown = _countdown > 0.1 ? 0.1 : _countdown;
	return true;
}

/**
 * @brief update is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	Bomb::update() {
	if (!active)
		return true;
	if (game.player->detonator) {
		if (Inputs::getKey(InputType::ACTION_2)) {
			explode({position.x, position.z});
		}
	} else {
		_countdown -= game.getDtTime();
		if (_countdown <= 0.0) {
			explode({position.x, position.z});
		}
	}
	return true;
}

/**
 * @brief PostUpdate is called each frame, after update().
 *
 * @return true if success
 * @return false if failure
 */
bool	Bomb::postUpdate() {
	if (!alive) {
		delete this;
	}
	return true;
}

/**
 * @brief draw is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	Bomb::draw(Gui &gui) {
	if (active) {
		gui.drawCube(Block::BOMB, getPos());
	}
	return true;
}

// -- Private Methods ----------------------------------------------------------

bool	Bomb::_propagationExplosion(glm::vec2 const place) {
	if (!game.positionInGame(glm::vec3(place.x, 0, place.y)))
		return false;

	std::vector<AEntity *>	&box = game.board[place.x][place.y];
	bool					continuePropagation = true;
	bool					addFire = false;

	if (box.size() == 0) {
		addFire = true;
	}
	else {
		std::vector<AEntity *>::iterator it = box.begin();
		while (it != box.end()) {
			if (*it == this) {
				++it;
				continue;
			}

			if ((*it)->blockPropagation) {
				continuePropagation = false;
			}

			if ((*it)->destructible) {
				addFire = true;
			}

			(*it)->takeDamage(1);

			if (it == box.end())
				continue;
			++it;
		}
	}
	// add fire
	if (addFire) {
		Fire *fire = new Fire(game);
		box.push_back(fire);
		fire->init();
	}

	return continuePropagation;
}

// -- Exceptions errors --------------------------------------------------------

Bomb::BombException::BombException()
: std::runtime_error("Bomb Exception") {}

Bomb::BombException::BombException(const char* whatArg)
: std::runtime_error(std::string(std::string("BombError: ") + whatArg).c_str()) {}
