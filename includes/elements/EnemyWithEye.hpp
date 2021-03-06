#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include "AEnemy.hpp"
#include "SceneGame.hpp"

#define ENEMY_WITH_EYE_STR "enemyWithEye"

/**
 * @brief This is an enemy object
 *
 * EnemyWithEye will follow a pattern and go to the player if it can view it
 */
class EnemyWithEye : public AEnemy {
private:
	EnemyWithEye();

	Direction::Enum	_playerDir;  /**< direction of the player (if view it) */

protected:
	virtual bool	_update();
	virtual bool	_postUpdate();
	virtual bool	_draw(Gui &gui);

	void			_updateModel();
	void			_updateAnimationState();

public:
	// Constructors
	explicit EnemyWithEye(SceneGame &game);
	~EnemyWithEye();
	EnemyWithEye(EnemyWithEye const &src);

	// Operators
	EnemyWithEye &operator=(EnemyWithEye const &rhs);

	// Methods
	virtual bool	init();
};
