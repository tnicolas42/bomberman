#pragma once

#include <vector>
#include "useGlm.hpp"

#include "AScene.hpp"
#include "SceneManager.hpp"

/* import all UI objects */
#include "ABaseUI.hpp"
#include "ButtonUI.hpp"
#include "ButtonImageUI.hpp"
#include "SliderUI.hpp"
#include "TextUI.hpp"
#include "RectUI.hpp"
#include "ImageUI.hpp"
#include "ScrollbarUI.hpp"
#include "EmptyMasterUI.hpp"
#include "TextInputUI.hpp"

#define VOID_COLOR glm::vec4 {-1 , -1, -1, -1}

/**
 * @brief Struct TransparentBox, store transparent ui pos/size for blur effect
 */
struct	TransparentBox {
	glm::vec2	pos;  /**< The box position */
	glm::vec2	size;  /**< The box size */
};

/**
 * @brief Scene object to re-implement in all scenes for menu
 *
 * this object contains functions to create buttons, images, ...
 */
class ASceneMenu : public AScene {
	public:
		// Members

		// Constructors
		ASceneMenu(Gui * gui, float const &dtTime);
		virtual ~ASceneMenu();
		ASceneMenu(ASceneMenu const &src);

		// Operators
		ASceneMenu &operator=(ASceneMenu const &rhs);
		friend std::ostream& operator<<(std::ostream& os, const ASceneMenu& myClass);

		// AScene methods
		virtual bool		init() = 0;
		virtual bool		update();
		virtual bool		draw();
		virtual void		load();
		virtual void		unload();

		/* add element */
		ButtonUI &			addButton(glm::vec2 pos, glm::vec2 size, std::string const & text);
		ButtonImageUI &		addButtonImage(glm::vec2 pos, glm::vec2 size, std::string const & filename,
			std::string const & filenameHover = "");
		SliderUI &			addSlider(glm::vec2 pos, glm::vec2 size, float min, float max, float val, float step);
		TextUI &			addText(glm::vec2 pos, glm::vec2 size, std::string const & text);
		TextUI &			addTitle(glm::vec2 pos, glm::vec2 size, std::string const & text);
		RectUI &			addRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color = VOID_COLOR,
			glm::vec4 borderColor = VOID_COLOR);
		ImageUI &			addImage(glm::vec2 pos, glm::vec2 size, std::string const & filename);
		ScrollbarUI &		addScrollbar(glm::vec2 pos, glm::vec2 size);
		EmptyMasterUI &		addEmptyMaster();
		TextInputUI &		addTextInput(glm::vec2 pos, glm::vec2 size, std::string const & defText);

		ButtonImageUI &		addExitButton();

		/* getter */
		ABaseUI &			getUIElement(uint32_t id);
		uint32_t			getNbUIElements() const;

	protected:
		std::vector<ABaseUI *>	_buttons;  /**< All UI elements (auto added with addXXX functions) */
		bool					_draw3dMenu;  /**< True if the menu background is in 3D */
		/* blur effect */
		bool	_blurEnabled;  /**< If enabled blur transparent ui */
		static	std::vector<uint8_t>		_aMaskData;  /**< Blur mask texture raw data */
		static	std::vector<TransparentBox>	_transparentBoxs;  /**< Transparents box to be blured */

		bool	_initBG();
		bool	_updateAlphaMask();
		bool	_updateAlphaMaskData();
};
