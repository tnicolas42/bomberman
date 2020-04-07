#ifndef SCENEGAME_HPP_
#define SCENEGAME_HPP_

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include "useGlm.hpp"

#include "ASceneMenu.hpp"
#include "AEntity.hpp"
#include "ACharacter.hpp"
#include "Bomb.hpp"
#include "Score.hpp"

#include "ABaseUI.hpp"
#include "TextUI.hpp"

#define NO_LEVEL -1  // value is no level loaded

class Player;
class AEnemy;

namespace GameState {
	enum Enum {
		PLAY,
		PAUSE,
		GAME_OVER,
		WIN,
	};
}

namespace EntityType {
	enum Enum {
		PLAYER,
		BOARD,
		BOARD_FLAG,
		ENEMY,
	};
}

/**
 * @brief This is the game Scene. In this scene, you can play to the game and load levels
 */
class SceneGame : public ASceneMenu {
private:
	SceneGame();
	// Members
	typedef AEntity*(*entityFuncPtr)(SceneGame &);
	struct Entity {
		EntityType::Enum	entityType;
		entityFuncPtr		entity;
	};
	static std::map<std::string, Entity> _entitiesCall;

	std::vector<SettingsJson *>	_mapsList;

	// Methods
	bool	_loadLevel(int32_t levelId);
	bool	_unloadLevel();
	bool	_initJsonLevel(int32_t levelId);
	void	_drawBoard();

protected:
	struct AllUI {
		ABaseUI *	timeLeftImg;  // TextUI
		ABaseUI *	timeLeftText;  // TextUI
		ABaseUI *	scoreImg;  // TextUI
		ABaseUI *	scoreText;  // TextUI
		ABaseUI *	lifeImg;  // ImageUI
		ABaseUI *	lifeText;  // TextUI
		ABaseUI *	speedImg;  // ImageUI
		ABaseUI *	speedText;  // TextUI
		ABaseUI *	bonusBombImg;  // ImageUI
		ABaseUI *	bonusBombText;  // TextUI
		ABaseUI *	bonusFlameImg;  // ImageUI
		ABaseUI *	bonusFlameText;  // TextUI
		ABaseUI *	bonusFlampassImg;  // ImageUI
		ABaseUI *	bonusWallpassImg;  // ImageUI
		ABaseUI *	bonusDetonatorImg;  // ImageUI
		ABaseUI *	bonusBombpassImg;  // ImageUI
		ABaseUI *	bonusShieldImg;  // ImageUI
		ABaseUI *	bonusShieldText;  // TextUI
	};
	AllUI			allUI;

	void			_initGameInfos();
	void			_updateGameInfos();
	bool			_initBonus();

public:
	// Members
	std::vector< std::vector< std::vector<AEntity *> > > board;
	std::vector< std::vector< std::vector<AEntity *> > > boardFly;
	Player						*player;
	std::vector<AEnemy *>		enemies;
	struct BonusValues {
		int64_t	chance;
		int64_t	nb;
	};
	std::unordered_map<std::string, BonusValues>	bonus;

	int							flags;
	glm::uvec2					size;
	int32_t						level;  // the current level ID (-1 for no level)
	GameState::Enum				state;
	uint32_t					levelEnemies;
	uint32_t					levelCrispies;
	float						levelTime;
	float						time;
	Score						score;

	// Constructors
	SceneGame(Gui * gui, float const &dtTime);
	virtual ~SceneGame();
	SceneGame(SceneGame const &src);

	// Operators
	SceneGame &operator=(SceneGame const &rhs);
	friend std::ostream& operator<<(std::ostream& os, const SceneGame& myClass);

	// Methods
	std::string		print() const;
	bool			clearFromBoard(AEntity *entity, glm::vec2 pos);
	bool			positionInGame(glm::vec3 pos, glm::vec3 sz = glm::vec3(1, 1, 1));

	// SceneGame methods
	virtual bool	init();
	virtual bool	update();
	virtual bool	postUpdate();
	virtual bool	draw();
	virtual void	load();
	virtual void	unload();
	bool			loadLevel(int32_t levelId);
	bool			save();

	// getter
	uint32_t		getNbLevel() const;
	std::string		getLevelName(int32_t levelId) const;
	std::string		getLevelImg(int32_t levelId) const;
	SettingsJson	&getSettingsLevel() const;

	// Exceptions
	class SceneGameException : public std::runtime_error {
	public:
		SceneGameException();
		explicit SceneGameException(const char* whatArg);
	};
};

#endif  // SCENEGAME_HPP_
