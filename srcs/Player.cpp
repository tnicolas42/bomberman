#include "Player.hpp"

// -- Constructors -------------------------------------------------------------

Player::Player() {
	type = Type::PLAYER;
}

Player::~Player() {
}

Player::Player(Player const &src) {
	*this = src;
}

// -- Operators ----------------------------------------------------------------

Player &Player::operator=(Player const &rhs) {
	if ( this != &rhs ) {
		ACharacter::operator=(rhs);
	}
	return *this;
}

// -- Methods ------------------------------------------------------------------

/**
 * update is called each frame.
 * @param d_time Delta Time
 */
bool	Player::update(float d_time) {
	std::cout << "Player updated at " << d_time << std::endl;
	return true;
}

/**
 * draw is called each frame.
 */
bool	Player::draw() {
	return true;
}

// -- Exceptions errors --------------------------------------------------------

Player::PlayerException::PlayerException()
: std::runtime_error("Player Exception") {}

Player::PlayerException::PlayerException(const char* what_arg)
: std::runtime_error(std::string(std::string("PlayerError: ") + what_arg).c_str()) {}
