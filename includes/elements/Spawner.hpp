#ifndef SPAWNER_HPP_
#define SPAWNER_HPP_

#include <iostream>
#include <stdexcept>
#include <vector>
#include "useGlm.hpp"
#include "AObject.hpp"
#include "AEnemy.hpp"

#define BIFROST_SHOW_DURATION	1

/**
 * @brief spawner objects
 */
class Spawner : public AObject {
private:
	std::vector<std::string>	_typeEnemy;  /**< Spawner enemy type */
	int64_t		_frequency;  /**< Frequency to spawn */
	float		_waitForSpawn;  /**< Wait time before first spawn */
	Model		*_model2;  /**< Second model, used for the transparent tube */
	float		_drawBifrostTime;  /**< Time left for the bifrost tube to display  */

	Spawner();

public:
	// Constructors
	explicit Spawner(SceneGame &game);
	~Spawner();
	Spawner(Spawner const &src);

	// Operators
	Spawner &operator=(Spawner const &rhs);

	// Getters & setters
	Spawner &setTypeEnemy(std::vector<std::string> typeEnemy);
	Spawner &addTypeEnemy(std::string typeEnemy);
	Spawner &setFrequency(int64_t frequency);

	// Methods
	bool	init();
	bool	update();
	bool	postUpdate();
	bool	draw(Gui &gui);

	// Exceptions
	/**
	 * @brief Spawner Exception
	 */
	class SpawnerException : public std::runtime_error {
	public:
		SpawnerException();
		/**
		 * @brief Construct a new Spawner Exception object
		 *
		 * @param whatArg Error message
		 */
		explicit SpawnerException(const char* whatArg);
	};
};

#endif  // SPAWNER_HPP_
