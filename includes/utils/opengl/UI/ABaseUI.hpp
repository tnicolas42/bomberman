#pragma once

#include "includesOpengl.hpp"
#include "Shader.hpp"
#include "TextRender.hpp"
#include "Inputs.hpp"

/* rect */
#define SHADER_RECT_2D_VS "./shaders/rect_2D_vs.glsl"
#define SHADER_RECT_2D_FS "./shaders/rect_2D_fs.glsl"
#define SHADER_RECT_2D_ROW_SZ 2

/* img */
#define SHADER_IMAGE_2D_VS "./shaders/image_2D_vs.glsl"
#define SHADER_IMAGE_2D_FS "./shaders/image_2D_fs.glsl"
#define SHADER_IMAGE_2D_ROW_SIZE 4

/**
 * @brief this is the text alignement (LEFT RIGTH or CENTER)
 */
namespace TextAlign {
	enum Enum {
		LEFT,
		CENTER,
		RIGHT,
	};
}

class ABaseMasterUI;

/**
 * @brief this is the base UI interface
 *
 * You can reinplement this class to create your UI elements.
 * This base class implement functions to draw rectangles, images, text, ...
 * You need to call some static functions to have functionnal UI
 *  - ABaseUI::init(...) once on startup
 *  - ABaseUI::staticUpdate() in each loop
 *  - ABaseUI::destroy() at the end
 */
class ABaseUI {
	public:
		/* static functions */
		static void init(glm::vec2 winSize, std::string const & fontName, uint32_t fontSize);
		static void destroy();
		static void staticUpdate();
		static void	setWinSize(glm::vec2 winSize);
		static void loadFont(std::string const & fontName, std::string const & filename, uint32_t fontSize);
		/* help */
		static void showHelp(bool show);
		static void setHelpFont(std::string fontName);
		static void setHelpToogleScancode(SDL_Scancode scancode);
		static void setHelpToogleInput(InputType::Enum input);
		static void setHelpTextScale(float scale);
		static void setHelpBorderSize(float borderSize);

		/* base functions */
		ABaseUI(glm::vec2 pos, glm::vec2 size);
		ABaseUI(ABaseUI const & src);
		virtual ~ABaseUI();

		ABaseUI & operator=(ABaseUI const & rhs);

		/* others */
		void			update();
		void			draw();

		/* listener */
		ABaseUI &	addButtonRightListener(bool * listener);
		ABaseUI &	addButtonLeftListener(bool * listener);

		/* setter */
		ABaseUI &	setKeyRightClickScancode(SDL_Scancode scancode);
		ABaseUI &	setKeyLeftClickScancode(SDL_Scancode scancode);
		ABaseUI &	setKeyRightClickInput(InputType::Enum input);
		ABaseUI &	setKeyLeftClickInput(InputType::Enum input);

		ABaseUI &	setEnabled(bool enabled);
		ABaseUI &	setPos(glm::vec2 pos);
		ABaseUI &	setPosOffset(glm::vec2 offset);
		ABaseUI &	addPosOffset(glm::vec2 offset);
		ABaseUI &	setSize(glm::vec2 size);
		ABaseUI &	setColor(glm::vec4 color);

		ABaseUI &	setBorderColor(glm::vec4 color);
		ABaseUI &	setBorderSize(float size);

		ABaseUI &	setMouseHoverColor(glm::vec4 color);
		ABaseUI &	setMouseClickColor(glm::vec4 color);

		ABaseUI &	setText(std::string const & text);
		ABaseUI &	setTextColor(glm::vec4 color);
		ABaseUI &	setTextFont(std::string const & font);
		ABaseUI &	setTextScale(float scale);
		ABaseUI &	setTextPadding(float padding);
		ABaseUI &	setTextAlign(TextAlign::Enum align);

		/* master */
		ABaseUI &	setMaster(ABaseMasterUI * master);

		/* check */
		bool					isPatriallyOutOfScreen() const;  // ret true if partially out of the screen
		bool					isTotallyOutOfScreen() const;  // ret true if totally out of the screen
		bool					isPartiallyOutOfMaster() const;  // ret true if partially out of the master element
		bool					isTotallyOutOfMaster() const;  // ret true if totally out of the screen element

		/* getter */
		bool					getMouseHover() const;
		bool					getMouseRightClick() const;
		bool					getMouseLeftClick() const;
		bool					isEnabled() const;
		glm::vec2 &				getPos();
		glm::vec2 const &		getPos() const;
		glm::vec2				getRealPos() const;  // get master + pos + offset
		glm::vec2 &				getSize();
		glm::vec2 const &		getSize() const;
		static Shader &			getRectShader();

		/* Exceptions */
	class UIException : public std::runtime_error {
		public:
			UIException();
			explicit UIException(const char* what_arg);
		};

	protected:
		ABaseUI();
		/* draw base function */
		// rect
		void			_drawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color1,
			glm::vec4 color2 = glm::vec4(1.0, 1.0, 1.0, 1.0), float factor = 1);
		void			_drawBorderRect(glm::vec2 pos, glm::vec2 size, float borderSize, glm::vec4 color);
		// text
		void			_drawText(glm::vec2 pos, glm::vec2 size, std::string const & font, float scale, std::string const & text,
			glm::vec4 color, TextAlign::Enum align, float padding);
		// img
		void			_loadImg(std::string const & filename, bool updateSize = true);
		void			_drawImg(glm::vec2 pos, glm::vec2 size, GLuint textureID, glm::vec4 color);

		// update function (redefined in child class)
		virtual void	_update() = 0;
		virtual void	_draw() = 0;

		// called by setWinSize
		virtual void	_resizeWin(glm::vec2 const & winScale2f, float winScale1f);

		// enable functionnalities
		bool			_enabled;
		// basics
		glm::vec2		_pos;
		glm::vec2		_posOffset;
		glm::vec2		_size;
		glm::vec4		_color;
		// border
		glm::vec4		_borderColor;
		float			_borderSize;
		// mouse effect
		glm::vec4		_mouseHoverColor;
		glm::vec4		_mouseClickColor;
		// text
		std::string		_text;
		glm::vec4		_textColor;
		std::string 	_textFont;
		float			_textScale;
		float			_textPadding;
		TextAlign::Enum	_textAlign;
		// image
		GLuint		_imgTextureID;
		glm::ivec2	_imgDefSize;

		/* info about mouse */
		bool			_isClickableUI;  // set to false for no buttons UI
		bool			_mouseHover;
		bool			_rightClick;
		SDL_Scancode	_keyRightClickBindScancode;
		InputType::Enum	_keyRightClickBindInput;
		bool			_leftClick;
		SDL_Scancode	_keyLeftClickBindScancode;
		InputType::Enum	_keyLeftClickBindInput;

		/* listener */
		bool *			_rightListener;
		bool *			_leftListener;

		/* master */
		// master element are an element that contains others
		ABaseMasterUI *	_master;

		/* global */
		static bool						_isInit;
		static std::vector<ABaseUI *>	_allUI;
		/* shaders */
		static glm::vec2				_winSize;
		static glm::mat4				_projection;  // projection matrix (orthogonal)
		/* rectangle */
		static Shader *					_rectShader;
		static GLuint					_rectVao;
		static GLuint					_rectVbo;
		static const float				_rectVertices[];
		/* text */
		static TextRender *				_textRender;
		static std::string				_defFont;
		/* image 2D */
		static Shader *					_imgShader;
		static GLuint					_imgVao;
		static GLuint					_imgVbo;
		static const float				_imgVertices[];
		/* help */
		static bool						_showHelp;
		static std::string				_helpFont;
		static float					_helpTextScale;
		static float					_helpBorderSize;
		static float					_helpPadding;
		static SDL_Scancode				_helpKeyBindScancode;
		static InputType::Enum			_helpKeyBindInput;

	private:
		void			_updateClick();
};
