#include "EnemyFollow.hpp"
#include "Player.hpp"

// -- Constructors -------------------------------------------------------------

EnemyFollow::EnemyFollow(SceneGame &game)
: AEnemy(game),
  _directionsOrder{Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT},
  _dirIdx(0),
  _findPlayer(false),
  _path()
{
	name = "EnemyFollow";
	_lastFindMs = getMs();
}

EnemyFollow::~EnemyFollow() {
	auto it = game.enemies.begin();
	while (it != game.enemies.end()) {
		if ((*it) == this)
			game.enemies.erase(it);
		else
			it++;
	}
}

EnemyFollow::EnemyFollow(EnemyFollow const &src) : AEnemy(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

EnemyFollow &EnemyFollow::operator=(EnemyFollow const &rhs) {
	if ( this != &rhs ) {
		AEnemy::operator=(rhs);
	}
	return *this;
}

// -- Methods ------------------------------------------------------------------

/**
 * @brief update is called each frame.
 *
 * @param dTime Delta Time
 * @return true if success
 * @return false if failure
 */
bool	EnemyFollow::_update(float const dTime) {
	// try to find a path to the player
	// after 1sec, 1 chance over 10 to relaunch path calculation
	if ((getMs() - _lastFindMs).count() > 1000 && (!_findPlayer || rand() % 100 < 10)) {
		_lastFindMs = getMs();
		_findPlayer = _getPathTo(game.player->getIntPos(), _path);
	}
	glm::vec3 lastPos = position;
	if (_path.size() == 0) {
		// arrived to destination
		_findPlayer = false;
	}
	else if (_followPath(dTime, _path) == false) {
		// blocked by a wall
		_findPlayer = false;
		_path.clear();
	}

	if (lastPos == position) {
		// if the enemy doesn't move
		_movePatternBasic(dTime, _directionsOrder, _dirIdx);
	}

	return true;
}

/**
 * @brief postUpdate is called each frame. After update()
 *
 * @return true if success
 * @return false if failure
 */
bool	EnemyFollow::_postUpdate() {
	return true;
}

/**
 * @brief draw is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	EnemyFollow::_draw(Gui &gui) {
	gui.drawCube(Block::IA, getPos());
	return true;
}
