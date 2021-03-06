#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include "AEnemy.hpp"
#include "SceneGame.hpp"

#define ENEMY_FOLLOW_STR "enemyFollow"

/**
 * @brief This is an enemy object
 *
 * EnemyFollow will always try to find the player and follow him
 */
class EnemyFollow : public AEnemy {
private:
	EnemyFollow();

	// player pathfinding info
	bool							_findPlayer;  /**< True if player found */
	std::deque<PathNode>			_path;  /**< Path to join player */
	// last calculation of path
	std::chrono::milliseconds		_lastFindMs;  /**< Last player that the enemy show the player */

protected:
	virtual bool	_update();
	virtual bool	_postUpdate();
	virtual bool	_draw(Gui &gui);

	void			_updateModel();
	void			_updateAnimationState();

public:
	// Constructors
	explicit EnemyFollow(SceneGame &game);
	~EnemyFollow();
	EnemyFollow(EnemyFollow const &src);

	// Operators
	EnemyFollow &operator=(EnemyFollow const &rhs);

	// Methods
	virtual bool	init();
};
