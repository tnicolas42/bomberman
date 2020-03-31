#include "EnemyWithEye.hpp"
#include "Player.hpp"

// -- Constructors -------------------------------------------------------------

EnemyWithEye::EnemyWithEye(SceneGame &game)
: AEnemy(game),
  _dir(Direction::UP),
  _playerDir(Direction::NO_DIRECTION)
{
	name = "EnemyWithEye";
}

EnemyWithEye::~EnemyWithEye() {
}

EnemyWithEye::EnemyWithEye(EnemyWithEye const &src) : AEnemy(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

EnemyWithEye &EnemyWithEye::operator=(EnemyWithEye const &rhs) {
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
bool	EnemyWithEye::_update(float const dTime) {
	if (_isBlocked())  // do nothing if blocked
		return true;
	Direction::Enum viewPlayerDir = _isPlayerVisible();
	if (viewPlayerDir != Direction::NO_DIRECTION) {
		_playerDir = viewPlayerDir;
		_dir = viewPlayerDir;
	}
	glm::vec3 tmpPos = position;
	if (tmpPos == _moveTo(_playerDir, dTime)) {
		_playerDir = Direction::NO_DIRECTION;
		_baseEnemyMove(dTime, _dir);
	}
	return true;
}

/**
 * @brief postUpdate is called each frame. After update()
 *
 * @return true if success
 * @return false if failure
 */
bool	EnemyWithEye::_postUpdate() {
	return true;
}

/**
 * @brief draw is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	EnemyWithEye::_draw(Gui &gui) {
	gui.drawCube(Block::IA, getPos(), size);
	return true;
}
