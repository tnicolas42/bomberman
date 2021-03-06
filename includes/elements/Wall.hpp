#ifndef WALL_HPP_
#define WALL_HPP_

#include <iostream>
#include <stdexcept>
#include "AObject.hpp"

#define WALL_STR "wall"

/**
 * @brief This is the wall object
 */
class Wall : public AObject {
private:
	Wall();
	// Members
	// graphic information.
	Block::Enum		_blockType;  /**< The type of the block */

public:
	// Constructors
	explicit Wall(SceneGame &game, Block::Enum blockType = Block::DURABLE_WALL);
	~Wall();
	Wall(Wall const &src);

	// Operators
	Wall &operator=(Wall const &rhs);

	// Methods
	bool	update();
	bool	draw(Gui &gui);

	// Exceptions
	/**
	 * @brief Wall Exception
	 */
	class WallException : public std::runtime_error {
	public:
		WallException();
		/**
		 * @brief Construct a new Wall Exception object
		 *
		 * @param whatArg Error message
		 */
		explicit WallException(const char* whatArg);
	};
};

#endif  // WALL_HPP_
