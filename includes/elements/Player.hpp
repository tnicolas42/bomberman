#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <iostream>
#include <stdexcept>
#include "ACharacter.hpp"
#include "SceneGame.hpp"

class Player : public ACharacter {
private:
	Player();
	// Methods
	void	_move(float const dTime);
	bool	_canMove(glm::vec3 pos);

public:
	// Members
	int			bombs;

	// Constructors
	explicit Player(SceneGame &game);
	~Player();
	Player(Player const &src);

	// Operators
	Player &operator=(Player const &rhs);

	// Methods
	bool	update(float const dTime);
	bool	draw(Gui &gui);

	// Exceptions
	class PlayerException : public std::runtime_error {
	public:
		PlayerException();
		explicit PlayerException(const char* whatArg);
	};
};

#endif  // PLAYER_HPP_
