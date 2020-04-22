#pragma once

#include <iostream>
#include <stdexcept>
#include <deque>
#include "ACharacter.hpp"
#include "SceneGame.hpp"

#define IS_ON_POS_OFFSET 0.2

#define INF_DIST 9999999

struct PathNode {
	Direction::Enum	dir;
	glm::ivec2		goal;
};

/**
 * @brief This is the base enemy object
 */
class AEnemy : public ACharacter {
private:
	AEnemy();

	struct DFSNode {
		uint32_t	dist;
		glm::ivec2	last;

		DFSNode() : dist(INF_DIST), last{-1, -1} {}
	};
	void			_dfsExplore(std::vector<std::vector<DFSNode>> & boardDFS,
		glm::ivec2 pos, glm::ivec2 dest, bool & find);
	bool			_getPathToDFS(glm::ivec2 dest, std::deque<PathNode> & path);

	void			_setMaxSize(bool enable);
	glm::vec3		_savedSize;

protected:
	// Members
	Direction::Enum	_dir;
	bool			_fisrtCall;  // true until the end of the first update call
	bool			_moveOnCenter;  // true if the enemy move on center (size < 1)

	virtual bool	_update() = 0;
	virtual bool	_postUpdate() = 0;
	virtual bool	_draw(Gui &gui) = 0;

	/* movements functions */
	bool			_baseEnemyMove(Direction::Enum & dir);
	bool			_movePatternBasic(std::vector<Direction::Enum> directionOrder,
		uint32_t & dirIdx);
	bool			_followPath(std::deque<PathNode> & path);

	/* check and calculation functions */
	Direction::Enum	_isPlayerVisible() const;
	bool			_isOn(glm::ivec2 dest, float offset = IS_ON_POS_OFFSET) const;
	bool			_getPathTo(glm::ivec2 dest, std::deque<PathNode> & path);
	bool			_isBlocked();

public:
	// Members
	int			bombs;
	int32_t		points;
	int			strength;  // number of damage if hit player

	// Constructors
	explicit AEnemy(SceneGame &game);
	~AEnemy();
	AEnemy(AEnemy const &src);

	// Operators
	AEnemy &operator=(AEnemy const &rhs);

	// Methods
	bool			update();
	bool			postUpdate();
	bool			draw(Gui &gui);
	virtual void	animEndCb(std::string animName);
	std::unordered_set<AEntity *>	getCollision(glm::vec3 dest) const;

	// Exceptions
	class EnemyException : public std::runtime_error {
	public:
		EnemyException();
		explicit EnemyException(const char* whatArg);
	};
};
