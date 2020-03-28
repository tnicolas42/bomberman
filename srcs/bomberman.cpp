#include <stdlib.h>
#include <unistd.h>
#include <chrono>

#include "bomberman.hpp"
#include "Logging.hpp"
#include "FileUtils.hpp"

SettingsJson s;

/**
 * @brief Init the logs
 *
 * You need to call this function only once at program startup.
 * This function init the logs functions
 */
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

/**
 * @brief Check if the program is valid (right files, folders, ...)
 *
 * @return false If program is invalid
 */
bool	checkPrgm() {
	/* list of required directories */
	std::vector<std::string> requiredDirs = {
		"bomberman-assets",
		"bomberman-assets/sounds",
		"bomberman-assets/textures",
		"bomberman-assets/skybox",
		s.s("mapsPath"),
	};

	/* list of required files */
	std::vector<std::string> requiredFiles = {
		s.j("font").s("file"),
	};

	for (auto && it : requiredDirs) {
		if (file::isDir(it) == false) {
			logErr(it << " directory doesn't exist");
			return false;
		}
	}
	for (auto && it : requiredFiles) {
		if (file::isFile(it) == false) {
			logErr(it << " file doesn't exist");
			return false;
		}
	}
	return true;
}

/**
 * @brief Create the pattern for master settings object & load settings
 *
 * @param filename the filename to read to set right values for settings
 * @return true if success
 * @return false if error
 */
bool	initSettings(std::string const & filename) {
	s.name("settings").description("main settings");

	s.add<std::string>("userDataFilename", "configs/userData.json").disableInFile(true);

	s.add<SettingsJson>("screen");
		s.j("screen").add<uint64_t>("fps", 60).setMin(30).setMax(120).setDescription("framerate");

	/* font */
	s.add<SettingsJson>("font");
		s.j("font").add<std::string>("file", "bomberman-assets/fonts/snakebold.TTF")
			.setDescription("this is the main font");
		s.j("font").add<uint64_t>("size", 20).setMin(10).setMax(50)
			.setDescription("default size for the text");

	/* colors */
	s.add<SettingsJson>("colors");
	// buttons
	s.j("colors").add<SettingsJson>("buttons");
		s.j("colors").j("buttons").add<double>("r", 0.07).setMin(0).setMax(1);
		s.j("colors").j("buttons").add<double>("g", 0.37).setMin(0).setMax(1);
		s.j("colors").j("buttons").add<double>("b", 0.8).setMin(0).setMax(1);
		s.j("colors").j("buttons").add<double>("a", 1.0).setMin(0).setMax(1);

	/* Audio */
	s.add<SettingsJson>("audio");
	s.j("audio").add<double>("Master volume", 1.0).setMin(0.0).setMax(1.0) \
		.setDescription("The global volume of the game.");
	s.j("audio").add<double>("Music volume", 1.0).setMin(0.0).setMax(1.0) \
		.setDescription("The volume of the music.");
	s.j("audio").add<double>("Sound volume", 1.0).setMin(0.0).setMax(1.0) \
		.setDescription("The volume of the sounds effects.");

	s.add<std::string>("mapsPath", "bomberman-assets/maps/").setDescription("folder with all maps");
	/* Graphics */
	s.add<SettingsJson>("graphics");
	s.j("graphics").add<bool>("fullscreen", false).setDescription("Display the game on fullscreen or not.");
	s.j("graphics").add<int64_t>("width", 1200).setMin(800).setMax(2560).setDescription("The resolution's width.");
	s.j("graphics").add<int64_t>("height", 800).setMin(600).setMax(1440).setDescription("The resolution's height.");

	// mouse sensitivity
	s.add<double>("mouse_sensitivity", 0.1).setMin(0.0).setMax(3.0) \
		.setDescription("Camera mouse sensitivity.");

	/* Debug */
	s.add<SettingsJson>("debug").setDescription("All debug settings");
	s.j("debug").add<bool>("showBaseBoard", true).setDescription("Show the base board");
	s.j("debug").add<bool>("showEntity", true).setDescription("Show the entities (player & enemy)");
	s.j("debug").add<bool>("showFlyHeight", false).setDescription("Show the fly height");

	try {
		if (file::isDir(filename)) {
			logWarn(filename << " is not the settings file, it is a directory");
		}
		else if (s.loadFile(filename) == false) {
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

/**
 * @brief Save the user data
 *
 * @param filename The file to save user data
 * @return true if success
 * @return false if error
 */
bool	saveSettings(std::string const & filename) {
	try {
		if (file::isDir(filename)) {
			logWarn("cannot write in " << filename << " -> it is a directory");
		}
		s.saveToFile(filename);
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
		return false;
	}
	return true;
}

/**
 * @brief Show usage for program
 *
 * @return false Return always false
 */
bool	usage() {
	std::cout << "usage: ./bomberman [-u] [--reset-settings]" << std::endl;
	std::cout << "\t" COLOR_BOLD "--reset-settings" COLOR_EOC ": "
		"reset all users settings before starting" << std::endl;
	std::cout << "\t" COLOR_BOLD "-u" COLOR_EOC ", " COLOR_BOLD "--usage" COLOR_EOC ": "
		"show usage" << std::endl;
	return false;
}

/**
 * @brief Parse args for program
 *
 * @param nbArgs number of arguments (argc - 1)
 * @param args arguments (av + 1)
 * @return false If need to quit
 */
bool	argparse(int nbArgs, char const ** args) {
	bool	reset_settings = false;
	int		i = 0;
	while (i < nbArgs) {
		if (strcmp(args[i], "--usage") == 0 || strcmp(args[i], "-u") == 0) {
			return usage();
		}
		else if (strcmp(args[i], "--reset-settings") == 0) {
			i++;
			reset_settings = true;
		}
		else {
			std::cout << "invalid argument: " << args[i] << std::endl;
			return usage();
		}
		i++;
	}
	if (reset_settings) {
		logInfo("reset all user settings...");
		file::rm(CONFIG_DIR);
	}
	return true;
}

/**
 * @brief Get the current time in ms
 *
 * @return std::chrono::milliseconds the ms object
 */
std::chrono::milliseconds getMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
}

/**
 * @brief Return time as a string formated m:ss
 *
 * @param time
 * @return std::string formated m:ss
 */
std::string					timeToString(float time) {
	int	minutes = time / 60;
	int	seconds = static_cast<int>(time) % 60;
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << seconds;
	return std::to_string(minutes) + ":" + ss.str();
}
