#ifndef AOBJECT_HPP_
#define AOBJECT_HPP_

#include <iostream>
#include <stdexcept>
#include "AEntity.hpp"

/**
 * @brief This is the base class for objects (Bomb, wall, ...)
 */
class AObject : public AEntity {
private:
	AObject();
public:
	// Constructors
	explicit AObject(SceneGame &game);
	virtual ~AObject();
	AObject(AObject const &src);

	// Operators
	AObject &operator=(AObject const &rhs);

	// Methods
	virtual bool	update(std::chrono::milliseconds dTime) = 0;
	virtual bool	draw() = 0;
	glm::vec2		getPos();

	// Exceptions
	class AObjectException : public std::runtime_error {
	public:
		AObjectException();
		explicit AObjectException(const char* whatArg);
	};
};

#endif  // AOBJECT_HPP_
