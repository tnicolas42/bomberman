#ifndef SOUND_HPP
# define SOUND_HPP

# include <SDL2/SDL.h>
# include <SDL2/SDL_mixer.h>
# include <string>
# include <unordered_set>
# include <map>
#include <stdexcept>

/**
 * @brief Sound class for AudioManager
 */
class Sound {
public:
	/**
	 * @brief Sound exception
	 */
	class SoundException: public std::runtime_error {
		public:
			SoundException();
			explicit SoundException(const char* what_arg);
			explicit SoundException(const std::string what_arg);
	};

	explicit Sound(std::string filename);
	~Sound();

	int											play(float volume, float env_volume, bool loop = false, int fadeIn = 0);
	void										pause();
	void										resume();
	void										stop(int fadeOut = 0);
	void										updateVolume(float volume);
	bool										channelFinished(int chan);

private:
	Sound();
	Sound(const Sound &src);
	Sound										&operator=(const Sound &src);

	Mix_Chunk *									_chunk;  /**< Sound object */
	std::unordered_set<int>						_currents_channels;  /**< Current sound channel */
	std::map<int, float>						_chan_volume;  /**< Channel volume */
};

#endif
