#include "Enemy.hpp"

// -- Constructors -------------------------------------------------------------

Enemy::Enemy(SceneGame &game) : ACharacter(game) {
	type = Type::ENEMY;
	name = "Enemy";
}

Enemy::~Enemy() {
}

Enemy::Enemy(Enemy const &src) : ACharacter(src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

Enemy &Enemy::operator=(Enemy const &rhs) {
	if ( this != &rhs ) {
		ACharacter::operator=(rhs);
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
bool	Enemy::update(std::chrono::milliseconds dTime) {
	std::cout << "Last Enemy updated at " << dTime.count() << std::endl;
	return true;
}

/**
 * @brief draw is called each frame.
 *
 * @return true if success
 * @return false if failure
 */
bool	Enemy::draw() {
	return true;
}

/**
 * @brief Static class to generate enemies
 *
 * @param game
 * @param rate Probability to generate an enemy is between 0 and 1.
 * @return Enemy*
 */
Enemy*	Enemy::generateEnemy(SceneGame &game, float rate) {
	srand(time(NULL));

	if (rate <= 0)
		return nullptr;
	if (rate >= 1)
		return new Enemy(game);

	int		percentRate = rand() % 100;
	if (percentRate > static_cast<int>(rate * 100))
		return nullptr;
	return new Enemy(game);
}

// -- Exceptions errors --------------------------------------------------------

Enemy::EnemyException::EnemyException()
: std::runtime_error("Enemy Exception") {}

Enemy::EnemyException::EnemyException(const char* whatArg)
: std::runtime_error(std::string(std::string("EnemyError: ") + whatArg).c_str()) {}
