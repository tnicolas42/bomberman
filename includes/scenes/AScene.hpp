#pragma once

#include <chrono>

#include "Gui.hpp"

/**
 * @brief base object to implement in all scenes
 */
class AScene {
	public:
		AScene(Gui * gui, float const &dtTime);
		AScene(AScene const & src);
		virtual ~AScene();

		virtual bool init() = 0;
		virtual bool update() = 0;
		virtual bool draw() = 0;
		virtual void load() = 0;
		virtual void unload() = 0;

		float const	&getDtTime() const;

		AScene & operator=(AScene const & rhs);

		// Exceptions
		class SceneException : public std::runtime_error {
		public:
			SceneException();
			explicit SceneException(const char* whatArg);
		};

	protected:
		AScene();

		Gui *	_gui;
		float const &_dtTime;
};
