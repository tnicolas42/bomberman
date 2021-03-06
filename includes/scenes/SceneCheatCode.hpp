#pragma once

#include <deque>
#include "ASceneMenu.hpp"

#define CHEATCODE_FONT					"cheatcode"
#define CHEATCODE_FONT_SCALE			1
#define CHEATCODE_COLOR					glm::vec4(0, 0, 0, 0.4)
#define CHEATCODE_DEF_TXT				"/"
#define CHEATCODE_TAB					"  "

/* logging */
#define CHEATCODE_TEXT_COlOR			glm::vec4(1, 1, 1, 1)
#define CHEATCODE_TEXT_COlOR_DEBUG		glm::vec4(0.7, 0.7, 0.7, 1)
#define CHEATCODE_TEXT_COlOR_INFO		CHEATCODE_TEXT_COlOR
#define CHEATCODE_TEXT_COlOR_SUCCESS	glm::vec4(0, 1, 0, 1)
#define CHEATCODE_TEXT_COlOR_WARN		glm::vec4(1, 1, 0, 1)
#define CHEATCODE_TEXT_COlOR_ERR		glm::vec4(1, 0, 0, 1)
#define CHEATCODE_TEXT_COlOR_FATAL		glm::vec4(1, 0, 0, 1)

#define CHEATCODE_CLEAR_ON_LOG			false  // clear screen when send message

#define _CHEATCODE_OBJECT reinterpret_cast<SceneCheatCode *>(SceneManager::getScene(SceneNames::CHEAT_CODE))
#define logDebugScreen(msg) { std::stringstream ss; ss << msg; \
								_CHEATCODE_OBJECT->logdebug(ss.str(), CHEATCODE_CLEAR_ON_LOG); }
#define logInfoScreen(msg) { std::stringstream ss; ss << msg; \
								_CHEATCODE_OBJECT->loginfo(ss.str(), CHEATCODE_CLEAR_ON_LOG); }
#define logSuccessScreen(msg) { std::stringstream ss; ss << msg; \
								_CHEATCODE_OBJECT->logsuccess(ss.str(), CHEATCODE_CLEAR_ON_LOG); }
#define logWarnScreen(msg) { std::stringstream ss; ss << msg; \
								_CHEATCODE_OBJECT->logwarn(ss.str(), CHEATCODE_CLEAR_ON_LOG); }
#define logErrScreen(msg) { std::stringstream ss; ss << msg; \
								_CHEATCODE_OBJECT->logerr(ss.str(), CHEATCODE_CLEAR_ON_LOG); }
#define logFatalScreen(msg) { std::stringstream ss; ss << msg; \
								_CHEATCODE_OBJECT->logfatal(ss.str(), CHEATCODE_CLEAR_ON_LOG); }

#define REGEX_INT	std::regex("^[ \n\t\r]*[-+]?\\d+[ \n\t\r]*$")
#define REGEX_UINT	std::regex("^[ \n\t\r]*\\+?\\d+[ \n\t\r]*$")
#define REGEX_FLOAT	std::regex("^[ \n\t\r]*[-+]?\\d+\\.?\\d*f?[ \n\t\r]*$")

namespace CheatcodeAction {
	// open or close cheatcode
	int const KEEP_OPEN =			0b00000000001;  // keep command line open
	int const CLOSE =				0b00000000010;  // close command line
	// setup txt in cheatcode
	int const TXT_RESET =			0b00000000100;  // reset command line text
	int const TXT_DEF =				0b00000001000;  // set command line text to default
	int const TXT_KEEP =			0b00000010000;  // keep the same command line text
	// if close, open text only mode
	int const CHEAT_NO_TXT_ONLY =	0b00000100000;  // don't open text only mode
	int const CHEAT_TXT_ONLY =		0b00001000000;  // open text only mode (only if CLOSE)
	// result of a command
	int const RESULT_SUCCESS =		0b00001000000;  // result of the command: success
	int const RESULT_ERROR =		0b00010000000;  // result of the command: error
};  // namespace CheatcodeAction

/**
 * @brief this is the cheat code command line
 */
class SceneCheatCode : public ASceneMenu {
	public:
		// Constructors
		SceneCheatCode(Gui * gui, float const &dtTime);
		virtual ~SceneCheatCode();
		SceneCheatCode(SceneCheatCode const &src);

		// Operators
		SceneCheatCode &operator=(SceneCheatCode const &rhs);

		// Methods
		virtual bool		init();
		virtual bool		update();
		virtual void		load();
		virtual void		unload();
		int					evalCommand(std::string const & command, bool ignoreHistory = false);
		void				clearAllLn();
		void				setText(std::string const & txt);
		std::string			getText() const;
		static bool			isLevelUnlocked(uint32_t levelId);
		int					unlockLevel(uint32_t levelId);

		/* log */
		void				logdebug(std::string const & msg, bool clear = false, bool logOnly = false);
		void				loginfo(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logsuccess(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logwarn(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logerr(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logfatal(std::string const & msg, bool clear = false, bool logOnly = false);

		bool				isCmdLnEnabled;  /**< True when command line is enabled */

	private:
		SceneCheatCode();
		/* for parser */
		void						_history();
		void						_autocompletion();
		std::vector<std::string>	_splitCommand(std::string const & command);
		bool						_isSpace(char c) const;
		bool						_isValidCommand(std::string const & name) const;
		int64_t						_toInt(std::string const & arg, bool & error, bool * isRelative = nullptr);
		uint64_t					_toUint(std::string const & arg, bool & error, bool * isRelative = nullptr);
		double						_toFloat(std::string const & arg, bool & error, bool * isRelative = nullptr);
		std::string					_getCommandName(std::string const & command, bool & isCommand);
		bool						_isLevelUnlocked(uint32_t levelId) const;

		/* commands definition */
		typedef int (SceneCheatCode::*execFnPtr)(std::vector<std::string> const &);
		/**
		 * @brief Command description
		 */
		struct Command {
			std::string	prototype;  /**< Prototype of the command */
			std::string	description;  /**< Description of the command */
			execFnPtr	exec;  /**< Function called by this command */
		};

		/* commands functions */
		// int _exec<cmd name>(std::vector<std::string> const & args);
		int					_execHelp(std::vector<std::string> const & args);
		int					_execClear(std::vector<std::string> const & args);
		int					_execLog(std::vector<std::string> const & args);
		int					_execTp(std::vector<std::string> const & args);
		int					_execGetbonus(std::vector<std::string> const & args);
		int					_execLoop(std::vector<std::string> const & args);
		int					_execExec(std::vector<std::string> const & args);
		int					_execSummon(std::vector<std::string> const & args);
		int					_execUnlock(std::vector<std::string> const & args);
		int					_execRmbonus(std::vector<std::string> const & args);
		int					_execRestart(std::vector<std::string> const & args);
		int					_execDebug(std::vector<std::string> const & args);
		int					_execVolume(std::vector<std::string> const & args);

		/* for lines */
		int					_addLine(std::string const & txt, glm::vec4 txtColor = CHEATCODE_TEXT_COlOR);
		void				_removeLastLine();
		/**
		 * @brief One line of text
		 */
		struct TextLine {
			ABaseUI *	ui;  /**< UI element of the line */
		};

		/* general variables */
		TextInputUI *					_commandLine;  /**< TextInput element */
		TextUI *						_infoCommandLine;  /**< Line with info about autocompletion */
		std::deque<TextLine>			_textLines;  /**< All textLines (add with _addLine & logXXX) */
		ABaseMasterUI *					_masterLines;  /**< Master element of lines */
		bool							_allLevelsUnlocked;  /**< True if `/unlock all` executed */
		std::vector<uint32_t>			_levelsUnlocked;  /**< All levels unlocked by command /unlock */

		/* history */
		std::deque<std::string>			_cmdHistory;  /**< History, first elem is last in history */
		std::string						_historySavedLine;  /**< The line wrote before go in history */
		int								_historyActID;  /**< Actual history ID */

		/* list of commands */
		std::map<std::string, Command>	_commandsList;  /**< List of all commands */
};
