#pragma once

#include <deque>
#include "ASceneMenu.hpp"
#include "TextInputUI.hpp"

#define CHEATCODE_FONT					"cheatcode"
#define CHEATCODE_FONT_SCALE			1
#define CHEATCODE_COLOR					glm::vec4(0, 0, 0, 0.5)
#define CHEATCODE_DEF_TXT				"/"
#define CHEATCODE_TAB					"    "

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

#define REGEX_INT std::regex("^[ \n\t\r]*[-+]?\\d+[ \n\t\r]*$")

namespace CheatcodeAction {
	enum Enum {
		KEEP_OPEN,
		KEEP_OPEN_RESET,
		KEEP_OPEN_DEF_TXT,
		KEEP_OPEN_KEEP_TXT,
		CLOSE,
		CLOSE_RESET,
		CLOSE_DEF_TXT,
		CLOSE_KEEP_TXT,
	};
};

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
		friend std::ostream& operator<<(std::ostream& os, const SceneCheatCode& myClass);

		// Methods
		virtual bool		init();
		virtual bool		update();
		virtual void		load();
		virtual void		unload();
		bool				evalCommand(std::string const & command);
		void				clearAllLn();
		void				setText(std::string const & txt);
		std::string			getText() const;

		/* log */
		void				logdebug(std::string const & msg, bool clear = false, bool logOnly = false);
		void				loginfo(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logsuccess(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logwarn(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logerr(std::string const & msg, bool clear = false, bool logOnly = false);
		void				logfatal(std::string const & msg, bool clear = false, bool logOnly = false);

		bool				isCmdLnEnabled;

	private:
		SceneCheatCode();
		/* for parser */
		std::vector<std::string>	_splitCommand(std::string const & command);
		bool						_isSpace(char c) const;
		bool						_isValidCommand(std::string const & name) const;
		int64_t						_toInt(std::string const & arg, bool & error) const;
		uint64_t					_toUint(std::string const & arg, bool & error) const;
		float						_toFloat(std::string const & arg, bool & error) const;

		/* commands definition */
		typedef CheatcodeAction::Enum (SceneCheatCode::*execFnPtr)(std::vector<std::string> const &);
		struct Command {
			std::string	prototype;
			std::string	description;
			execFnPtr	exec;
		};

		/* commands functions */
		// CheatcodeAction::Enum _exec<cmd name>(std::vector<std::string> const & args);
		CheatcodeAction::Enum	_execHelp(std::vector<std::string> const & args);
		CheatcodeAction::Enum	_execClear(std::vector<std::string> const & args);
		CheatcodeAction::Enum	_execLog(std::vector<std::string> const & args);
		CheatcodeAction::Enum	_execTp(std::vector<std::string> const & args);

		/* for lines */
		void				_addLine(std::string const & txt, glm::vec4 txtColor = CHEATCODE_TEXT_COlOR);
		void				_removeLastLine();
		struct TextLine {
			ABaseUI *	ui;
		};

		/* general variables */
		TextInputUI *					_commandLine;
		std::deque<TextLine>			_textLines;

		/* history */
		std::deque<std::string>			_cmdHistory;  // first elem is last in history
		std::string						_historySavedLine;
		int								_historyActID;

		/* list of commands */
		std::map<std::string, Command>	_commandsList;
};
