#include <stdlib.h>
#include <unistd.h>
#include <chrono>

#include "bomberman.hpp"
#include "Logging.hpp"

SettingsJson s;
SettingsJson userData;

void	initLogs() {
	// init logging
	#if DEBUG
		logging.setLoglevel(LOGDEBUG);
		logging.setPrintFileLine(LOGWARN, true);
		logging.setPrintFileLine(LOGERROR, true);
		logging.setPrintFileLine(LOGFATAL, true);
	#else
		logging.setLoglevel(LOGINFO);
	#endif
	logDebug("using debug mode");
}

bool	initSettings(std::string const & filename) {
	s.name("settings").description("main settings");

	s.add<std::string>("userDataFilename", "configs/userData.json").disableInFile(true);

	s.add<SettingsJson>("screen");
		s.j("screen").add<uint64_t>("fps", 60).setMin(30).setMax(120).setDescription("framerate");

	/* font */
	s.add<SettingsJson>("font");
		s.j("font").add<std::string>("file", "bomberman-assets/fonts/Roboto-Regular.ttf")
			.setDescription("this is the main font");
		s.j("font").add<uint64_t>("size", 40).setMin(10).setMax(50)
			.setDescription("default size for the text");

	/* colors */
	s.add<SettingsJson>("colors");
	// buttons
	s.j("colors").add<SettingsJson>("buttons");
		s.j("colors").j("buttons").add<double>("r", 0.2).setMin(0).setMax(1);
		s.j("colors").j("buttons").add<double>("g", 0.2).setMin(0).setMax(1);
		s.j("colors").j("buttons").add<double>("b", 0.2).setMin(0).setMax(1);
		s.j("colors").j("buttons").add<double>("a", 1.0).setMin(0).setMax(1);


	s.add<SettingsJson>("audio");
	s.j("audio").add<double>("masterVolume", 1.0).setMin(0.0).setMax(1.0) \
		.setDescription("The global volume of the game.");
	s.j("audio").add<double>("musicVolume", 1.0).setMin(0.0).setMax(1.0) \
		.setDescription("The volume of the music.");
	s.j("audio").add<double>("soundVolume", 1.0).setMin(0.0).setMax(1.0) \
		.setDescription("The volume of the sounds effects.");

	s.add<std::string>("mapsPath", "bomberman-assets/maps/").setDescription("folder with all maps");

	try {
		if (s.loadFile(filename) == false) {
			// warning when loading settings
			return false;
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
		return false;
	}
	return true;
}

bool	initUserData(std::string const & filename) {
	userData.name("userData").description("all data saved");

	userData.add<uint64_t>("highScore", 0);  // example

	try {
		if (userData.loadFile(filename) == false) {
			// warning when loading settings
			return false;
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		// file doesn't exist, create it at the end
		logDebug("the file " << filename << " doesn't exist for now");
		return false;
	}
	return true;
}

bool	saveUserData(std::string const & filename) {
	try {
		userData.saveToFile(filename);
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
		return false;
	}
	return true;
}

std::chrono::milliseconds getMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
}
