#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include <iostream>
#include <stdexcept>
#include "ACharacter.hpp"
#include "SceneGame.hpp"

class Enemy : public ACharacter {
private:
	Enemy();

public:
	// Members
	int			bombs;
	SceneGame	*game;

	// Constructors
	explicit Enemy(SceneGame &game);
	~Enemy();
	Enemy(Enemy const &src);

	// Operators
	Enemy &operator=(Enemy const &rhs);

	// Methods
	bool			update(std::chrono::milliseconds d_time);
	bool			draw();
	bool			isDestructable();
	bool			blockPropagation();
	bool			isAlive();
	static Enemy*	generateEnemy(SceneGame &game, float rate);

	// Exceptions
	class EnemyException : public std::runtime_error {
	public:
		EnemyException();
		explicit EnemyException(const char* what_arg);
	};
};

#endif  // ENEMY_HPP_
