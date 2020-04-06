#include "EnemyCrispy.hpp"
#include "Player.hpp"

// -- Constructors -------------------------------------------------------------

EnemyCrispy::EnemyCrispy(SceneGame &game)
: AEnemy(game),
  _isWall(true),
  _playerDir(Direction::NO_DIRECTION),
  _lastPayerSeenMs(0)
{
	name = "EnemyCrispy";
}

EnemyCrispy::~EnemyCrispy() {
	auto it = game.enemies.begin();
	while (it != game.enemies.end()) {
		if ((*it) == this)
			game.enemies.erase(it);
		else
			it++;
	}
}

EnemyCrispy::EnemyCrispy(EnemyCrispy const &src) : AEnemy(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

EnemyCrispy &EnemyCrispy::operator=(EnemyCrispy const &rhs) {
	if ( this != &rhs ) {
		logWarn("EnemyCrispy operator= called (not implemented)");
		AEnemy::operator=(rhs);
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
bool	EnemyCrispy::_update() {
	if (_isBlocked())  // do nothing if blocked
		return true;

	/* retransform to a wall */
	if (getMs().count() - _lastPayerSeenMs > TIME_BEFORE_TRANSFORM_TO_WALL &&
		_isOn(getIntPos(), game.getDtTime() * speed * 3))
	{
		position.x = getIntPos().x;
		position.z = getIntPos().y;
		_isWall = true;
	}

	/* try to find player */
	Direction::Enum viewPlayerDir = _isPlayerVisible();
	if (viewPlayerDir != Direction::NO_DIRECTION) {
		_playerDir = viewPlayerDir;
		_dir = viewPlayerDir;
		_isWall = false;
		_lastPayerSeenMs = getMs().count();
	}

	/* move */
	if (!_isWall) {
		glm::vec3 tmpPos = position;
		if (tmpPos == _moveTo(_playerDir)) {
			_playerDir = Direction::NO_DIRECTION;
			_baseEnemyMove(_dir);
		}
	}
	return true;
}

/**
 * @brief postUpdate is called each frame. After update()
 *
 * @return true if success
 * @return false if failure
 */
bool	EnemyCrispy::_postUpdate() {
	return true;
}

/**
 * @brief draw is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	EnemyCrispy::_draw(Gui &gui) {
	if (_isWall)
		gui.drawCube(Block::DESTRUCTIBLE_WALL, getPos());
	else
		gui.drawCube(Block::IA, getPos(), size);
	return true;
}
