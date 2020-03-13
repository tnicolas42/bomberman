#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <iostream>
#include <stdexcept>
#include <chrono>
#include "useGlm.hpp"
#include "Logging.hpp"
#include "Gui.hpp"
class SceneGame;

namespace Category {
	enum Enum {
		STATIC,
		MOBILE,
	};
}
namespace Type {
	enum Enum {
		PLAYER,
		ENEMY,
		BOMB,
		FIRE,
		WALL,
		CRISPY,
		FLAG,
		END,
	};
}  // namespace Type

/**
 * @brief This is the base class for entity (Charactere, Objects, ...)
 */
class AEntity {
private:
	AEntity();

protected:
	float			_timeToDie;

public:
	// Members
	bool			active;
	bool			alive;
	Category::Enum	category;
	std::string		name;
	Type::Enum		type;
	SceneGame		&game;
	bool			destructible;
	bool			blockPropagation;
	glm::vec3		position;

	// Constructors
	explicit AEntity(SceneGame &game);
	virtual ~AEntity();
	AEntity(const AEntity &src);

	// Operators
	AEntity			&operator=(const AEntity &rhs);

	// Methods
	virtual bool		update(const float dTime) = 0;
	virtual bool		postUpdate();
	virtual bool		draw(Gui &gui) = 0;
	virtual glm::vec3	getPos() = 0;
	virtual void		takeDamage(const int damage) = 0;
};

#endif  // ENTITY_HPP_
