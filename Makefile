# Makefile
# for each project, you can configure the Makefile
# /!\ each variables with -> need to be changed /!\

# project global config
#	-> NAME
#	ARGS
#	CC
#	CFLAGS
#	DEBUG_FLAGS
#	NODEBUG_FLAGS
#	DEPFLAGS
#	DEFINE

# source files
#	SRCS_DIR
#	OBJS_DIR
#	INC_DIR
#	LIBS_DIR
#	DEP_DIR
#	DEBUG_DIR
#	-> SRC
#	-> HEAD

# libs configuration
#	-> LIBS_SRC_C
#	-> LIBS_SRC_CPP
#	-> LIBS_HEAD
#	-> LIBS_FLAGS
#	-> LIBS_FLAGS_OSX
#	-> LIBS_FLAGS_LINUX
#	-> LIBS_INC
#	-> UNCOMPILED_LIBS
#	NEED_MAKE

# configure file
#	-> CONFIGURE

# linter config
#	CONFIGURE_LINTER
#	LINTER
#	LINTER_RULES

# pre-commit & pre-push files
#	PRE_COMMIT_FILE
#	PRE_COMMIT
#	PRE_PUSH_FILE
#	PRE_PUSH

# after this, to install the project:
#	make install  # this command will exectute ./configure.sh so you need to create this file
# to compile & run
#	make exec -j8 ARGS="--usage"
# to make without make user libs
#	make NEED_MAKE=""
# to get help:
#	make help

# to compile with debug mode:
#	make DEBUG=1 -j8

################################################################################
# project global config

# executable name
NAME = bomberman
# args (./NAME ARGS) (make exec ARGS="-v" -> ./name -v)
ARGS =
# compiler (g++ or clang++)
CC = g++
# flags for compilation
CFLAGS = -Ofast -std=c++17 -Wall -Wextra -Wno-deprecated
# flags only for debug mode (make DEBUG=1)
DEBUG_FLAGS = -g3 -DDEBUG=true -fsanitize=address
# classic flags
NODEBUG_FLAGS = -Werror
# flags to create the .d files
DEPFLAGS = -MT $@ -MD -MP -MF $(DEP_DIR)/$*.Td
# to send define to program (make DEFINE="WIDTH=120")
DEFINE =


################################################################################
# source files

# directory names
SRCS_DIR	= srcs
OBJS_DIR	= objs
INC_DIR		= includes
LIBS_DIR	= libs
BREW_DIR    = /opt/homebrew
DEP_DIR		= .dep
# this will create a DEBUG file if we are in debug mode (make DEBUG=1)
DEBUG_DIR	= $(DEP_DIR)

# SRCS_DIR/SRC
SRC =	main.cpp \
		bomberman.cpp \
		AEntity.cpp \
\
		ACharacter.cpp \
		AEnemy.cpp \
		AEnemy_pathfinding.cpp \
		elements/Player.cpp \
\
		elements/Spawner.cpp \
		elements/EnemyBasic.cpp \
		elements/EnemyFollow.cpp \
		elements/EnemyWithEye.cpp \
		elements/EnemyFly.cpp \
		elements/EnemyCrispy.cpp \
		elements/EnemyFrog.cpp \
\
		AObject.cpp \
		elements/Bomb.cpp \
		elements/Bonus.cpp \
		elements/Fire.cpp \
		elements/Wall.cpp \
		elements/Crispy.cpp \
		elements/End.cpp \
		elements/Flag.cpp \
		elements/Score.cpp \
		elements/Save.cpp \
\
		scenes/SceneManager.cpp \
		scenes/AScene.cpp \
		scenes/ASceneMenu.cpp \
		scenes/SceneMainMenu.cpp \
		scenes/SceneLoadGame.cpp \
		scenes/SceneDifficulty.cpp \
		scenes/SceneLevelSelection.cpp \
		scenes/SceneGame/SceneGame.cpp \
		scenes/SceneGame/SceneGame_init.cpp \
		scenes/SceneGame/SceneGame_loadlevel.cpp \
		scenes/SceneGame/SceneGame_update.cpp \
		scenes/SceneGame/SceneGame_draw.cpp \
		scenes/ScenePause.cpp \
		scenes/SceneGameOver.cpp \
		scenes/SceneVictory.cpp \
		scenes/SceneExit.cpp \
		scenes/SceneSettings.cpp \
		scenes/SceneCheatCode.cpp \
		scenes/SceneCheatCode_exec.cpp \
		scenes/SceneEndGame.cpp \
		scenes/SceneDebug.cpp \
		scenes/SceneLoading.cpp \
		scenes/SceneHelp.cpp \
\
		audio/AudioManager.cpp \
		audio/Music.cpp \
		audio/Sound.cpp \
\
		gui/Gui.cpp \
		gui/TextureManager.cpp \
		gui/ModelsManager.cpp \
		gui/BoxCollider.cpp \
\
		utils/Logging.cpp \
		utils/SettingsJson.cpp \
		utils/FileUtils.cpp \
		utils/Stats.cpp \
\
		utils/opengl/Inputs.cpp \
		utils/opengl/Texture.cpp \
		utils/opengl/Shader.cpp \
		utils/opengl/Camera.cpp \
		utils/opengl/debug.cpp \
		utils/opengl/Material.cpp \
		utils/opengl/TextRender.cpp \
		utils/opengl/ImageAtlasRender.cpp \
		utils/opengl/Skybox.cpp \
		utils/opengl/assimpUtils.cpp \
		utils/opengl/Mesh.cpp \
		utils/opengl/Model.cpp \
		utils/opengl/OpenGLModel.cpp \
		utils/opengl/ETransform.cpp \
\
		utils/opengl/UI/ABaseUI.cpp \
		utils/opengl/UI/ABaseUI_static.cpp \
		utils/opengl/UI/ABaseUI_utils.cpp \
		utils/opengl/UI/ABaseMasterUI.cpp \
		utils/opengl/UI/ButtonUI.cpp \
		utils/opengl/UI/ButtonImageUI.cpp \
		utils/opengl/UI/SliderUI.cpp \
		utils/opengl/UI/TextUI.cpp \
		utils/opengl/UI/RectUI.cpp \
		utils/opengl/UI/ImageUI.cpp \
		utils/opengl/UI/ScrollbarUI.cpp \
		utils/opengl/UI/EmptyMasterUI.cpp \
		utils/opengl/UI/TextInputUI.cpp \

# INC_DIR/HEAD
HEAD =	bomberman.hpp \
		AEntity.hpp \
\
		ACharacter.hpp \
		AEnemy.hpp \
		elements/Player.hpp \
\
		elements/Spawner.hpp \
		elements/EnemyBasic.hpp \
		elements/EnemyFollow.hpp \
		elements/EnemyWithEye.hpp \
		elements/EnemyFly.hpp \
		elements/EnemyCrispy.hpp \
		elements/EnemyFrog.hpp \
\
		AObject.hpp \
		elements/Bomb.hpp \
		elements/Bonus.hpp \
		elements/Fire.hpp \
		elements/Wall.hpp \
		elements/Crispy.hpp \
		elements/End.hpp \
		elements/Flag.hpp \
		elements/Score.hpp \
		elements/Save.hpp \
\
		scenes/SceneManager.hpp \
		scenes/AScene.hpp \
		scenes/ASceneMenu.hpp \
		scenes/SceneMainMenu.hpp \
		scenes/SceneLoadGame.hpp \
		scenes/SceneDifficulty.hpp \
		scenes/SceneLevelSelection.hpp \
		scenes/SceneGame.hpp \
		scenes/ScenePause.hpp \
		scenes/SceneGameOver.hpp \
		scenes/SceneVictory.hpp \
		scenes/SceneExit.hpp \
		scenes/SceneSettings.hpp \
		scenes/SceneCheatCode.hpp \
		scenes/SceneEndGame.hpp \
		scenes/SceneDebug.hpp \
		scenes/SceneLoading.hpp \
		scenes/SceneHelp.hpp \
\
		audio/AudioManager.hpp \
		audio/Music.hpp \
		audio/Sound.hpp \
\
		gui/Gui.hpp \
		gui/TextureManager.hpp \
		gui/ModelsManager.hpp \
		gui/BoxCollider.hpp \
\
		utils/Logging.hpp \
		utils/SettingsJson.hpp \
		utils/FileUtils.hpp \
		utils/useGlm.hpp \
		utils/Stats.hpp \
\
		utils/opengl/Inputs.hpp \
		utils/opengl/Texture.hpp \
		utils/opengl/Shader.hpp \
		utils/opengl/Camera.hpp \
		utils/opengl/debug.hpp \
		utils/opengl/Material.hpp \
		utils/opengl/TextRender.hpp \
		utils/opengl/ImageAtlasRender.hpp \
		utils/opengl/Skybox.hpp \
		utils/opengl/assimpUtils.hpp \
		utils/opengl/Mesh.hpp \
		utils/opengl/Model.hpp \
		utils/opengl/OpenGLModel.hpp \
		utils/opengl/ETransform.hpp \
\
		utils/opengl/UI/ABaseUI.hpp \
		utils/opengl/UI/ButtonUI.hpp \
		utils/opengl/UI/ABaseMasterUI.hpp \
		utils/opengl/UI/ButtonImageUI.hpp \
		utils/opengl/UI/SliderUI.hpp \
		utils/opengl/UI/TextUI.hpp \
		utils/opengl/UI/RectUI.hpp \
		utils/opengl/UI/ImageUI.hpp \
		utils/opengl/UI/ScrollbarUI.hpp \
		utils/opengl/UI/EmptyMasterUI.hpp \
		utils/opengl/UI/TextInputUI.hpp \


################################################################################
# libs configuration

# for c libs
LIBS_SRC_C =	glad/glad.c \

# for cpp libs
LIBS_SRC_CPP =

# headers for c & cpp libs
LIBS_HEAD =	glad/glad.h \
			KHR/khrplatform.h \
			stb_image.h \

# all flags for libs
LIBS_FLAGS =	-L $(BREW_DIR)/lib -l SDL2 -l SDL2_mixer -l assimp \
				-L $(BREW_DIR)/opt/freetype/lib -lfreetype \
				-lboost_filesystem \

# flags for libs on OSX only
LIBS_FLAGS_OSX =	-rpath $(BREW_DIR)/lib -framework OpenGL

# flags for libs on LINUX only
LIBS_FLAGS_LINUX =	-Wl,-rpath,/usr/lib/x86_64-linux-gnu -lGL -lGLU \
					-lboost_system \
					-l z -L libs/assimp-5.0.1/build/contrib/irrXML -l IrrXML \
					-L libs/assimp-5.0.1/build/code \

# includes dir for external libs
LIBS_INC =	$(BREW_DIR)/include \
			$(LIBS_DIR) \
			/usr/local/opt/freetype/include/freetype2 \
			$(BREW_DIR)/opt/freetype/include/freetype2 \
			/usr/include/freetype2 \
			/usr/include/assimp \
			/usr/include/SDL2 \
			libs/assimp-5.0.1/include/ \
			libs/assimp-5.0.1/build/include/ \

# libs created by user
UNCOMPILED_LIBS =

# libs that need to be maked
NEED_MAKE =	$(UNCOMPILED_LIBS)

################################################################################
# configure file

# installation script (installation for all libs)
define CONFIGURE
#!/bin/bash

mkdir -p $(LIBS_DIR) $(LIBS_DIR)/glad $(LIBS_DIR)/KHR
# json lib
wget -O $(LIBS_DIR)/json.hpp https://raw.githubusercontent.com/nlohmann/json/8d92ca8/single_include/nlohmann/json.hpp
# stb_image lib
wget -O $(LIBS_DIR)/glad/glad.h https://raw.githubusercontent.com/tnicolas42/libs/master/cpp/OpenGL/glad/glad/glad.h
wget -O $(LIBS_DIR)/glad/glad.c https://raw.githubusercontent.com/tnicolas42/libs/master/cpp/OpenGL/glad/glad/glad.c
wget -O $(LIBS_DIR)/KHR/khrplatform.h https://raw.githubusercontent.com/tnicolas42/libs/master/cpp/OpenGL/glad/KHR/khrplatform.h
wget -O $(LIBS_DIR)/stb_image.h https://raw.githubusercontent.com/nothings/stb/0224a44/stb_image.h

# Linux
if [[ "$$OSTYPE" == "linux-gnu" ]]; then
	echo "install linux dependencies"
	sudo apt-get update -y
	# boost
	sudo apt-get -y install libboost-all-dev
	# glm
	sudo apt-get -y install libglm-dev
	# freetype (for text)
	sudo apt-get -y install libfreetype6-dev libfontconfig1-dev
	# sdl2
	sudo apt-get -y install libsdl2-dev;
	# sdl2_mixer
	sudo apt-get -y install libmikmod-dev libfishsound1-dev libsmpeg-dev liboggz2-dev libflac-dev libfluidsynth-dev libsdl2-mixer-dev
	# for docs
	sudo apt-get -y install doxygen graphviz
	# assimp 5.0.1
	#   download assimp
	sudo apt-get install -y wget tar cmake zlib1g-dev;
	wget -O $(LIBS_DIR)/v5.0.1.tar.gz https://github.com/assimp/assimp/archive/v5.0.1.tar.gz;
	tar xzf $(LIBS_DIR)/v5.0.1.tar.gz -C libs;
	rm $(LIBS_DIR)/v5.0.1.tar.gz;
	#   build assimp
	cd libs/assimp-5.0.1;
	mkdir build; cd build;
	export CC=`which gcc`;
	cmake .. -G "Unix Makefiles" -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_STATIC=ON -DASSIMP_BUILD_STATIC_LIB=ON;
	make -j4;
	cd ../../..;

# Mac OSX
elif [[ "$$OSTYPE" == "darwin"* ]]; then
	echo "install osx dependencies";
	# boost
	brew install boost
	# glm
	brew install glm
	# sdl2
	brew install sdl2;
	# sdl2_mixer
	brew install sdl2_mixer;
	# assimp
	brew install assimp;
	brew upgrade assimp;
	# freetype2
	brew install freetype2
	# icon for .app file
	brew install fileicon
	# for docs
	brew install doxygen
	brew install graphviz
fi

exit 0
endef
export CONFIGURE

################################################################################
# linter config

# installation script for cpp linter
define CONFIGURE_LINTER
#!/bin/sh

if [ ! -d ~/.cpplinter ]; then
	git clone https://github.com/tnicolas42/cpplinter.git ~/.cpplinter
	echo "source ~/.cpplinter/alias.zsh" >> ~/.zshrc
else
	echo "linter already installed"
fi
endef
export CONFIGURE_LINTER

# download the cpp linter (https://github.com/isocpp/CppCoreGuidelines)
# set command to launch linter on LINTER
# add rules for linter in LINTER_RULES
LINTER = $(CPPLINT)
LINTER_RULES =	--filter=-whitespace/tab,-legal/copyright,-build/c++11,-whitespace/newline,-readability/braces,-whitespace/indent,-build/include_what_you_use,-build/header_guard,-runtime/references,-runtime/threadsafe_fn,-whitespace/braces,-whitespace/parens,-runtime/indentation_namespace \
				--linelength=120 --quiet

################################################################################
# pre-commit & pre-push files

# pre-commit rules
PRE_COMMIT_FILE = .git/hooks/pre-commit
define PRE_COMMIT
#!/bin/zsh

$(MAKE) $(MAKE_OPT) lint
res=$$?

exit $${res}
endef
export PRE_COMMIT

# pre-push rules
PRE_PUSH_FILE = .git/hooks/pre-push
define PRE_PUSH
#!/bin/zsh
exit 0
endef
export PRE_PUSH

################################################################################
# automatic defined variables

# name in majuscule
PROJECT_NAME = $(shell echo $(NAME) | tr a-z A-Z)  # name in MAJUSCULE

MAKE_OPT = --no-print-directory

# os specific flags for libs
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	LIBS_FLAGS += $(LIBS_FLAGS_OSX)
else
	LIBS_FLAGS += $(LIBS_FLAGS_LINUX)
endif

# setup correct shell
ifneq (,$(wildcard /bin/zsh))
	SHELL := /bin/zsh
else ifneq (,$(wildcard /bin/bash))
	SHELL := /bin/bash
else
	SHELL := /bin/sh
endif

# setup debug mode or normal mode
ifneq ($(DEBUG),)
	CFLAGS := $(CFLAGS) $(DEBUG_FLAGS)
else
	CFLAGS := $(CFLAGS) $(NODEBUG_FLAGS)
endif

# send define to program
CFLAGS := $(CFLAGS) $(addprefix -D, $(DEFINE))


HEADS		= $(addprefix $(INC_DIR)/, $(HEAD)) $(addprefix $(LIBS_DIR)/, $(LIBS_HEAD))
SRCS		= $(addprefix $(SRCS_DIR)/, $(SRC)) \
			  $(addprefix $(LIBS_DIR)/, $(LIBS_SRC_C)) \
			  $(addprefix $(LIBS_DIR)/, $(LIBS_SRC_CPP))
OBJS		= $(addprefix $(OBJS_DIR)/, $(SRC:.cpp=.o)) \
			  $(addprefix $(OBJS_DIR)/, $(LIBS_SRC_C:.c=.o)) \
			  $(addprefix $(OBJS_DIR)/, $(LIBS_SRC_CPP:.cpp=.o))
DEPFILES	= $(addprefix $(DEP_DIR)/, $(SRC:.cpp=.d)) \
			  $(addprefix $(DEP_DIR)/, $(LIBS_SRC_C:.c=.d)) \
			  $(addprefix $(DEP_DIR)/, $(LIBS_SRC_CPP:.cpp=.d))
INC			= -I . $(addprefix -I , $(sort $(dir $(HEADS)))) \
			  $(addprefix -I , $(LIBS_INC)) \
			  $(addprefix -I , $(UNCOMPILED_LIBS)) \
			  $(addprefix -I , $(addsuffix /$(INC_DIR), $(UNCOMPILED_LIBS))) \

NORMAL = "\x1B[0m"
RED = "\x1B[31m"
GREEN = "\x1B[32m"
YELLOW = "\x1B[33m"
BLUE = "\x1B[34m"
MAGENTA = "\x1B[35m"
CYAN = "\x1B[36m"
WHITE = "\x1B[37m"
BOLD = "\e[1m"
LIGHT = "\e[2m"
ITALIC = "\e[3m"
ULINE = "\e[4m"

START = @printf $(GREEN)$(BOLD)"$(PROJECT_NAME)\n--------------------\n"$(NORMAL)
END = @printf $(GREEN)$(BOLD)"--------------------\n"$(NORMAL)

################################################################################
# make rules

all:
# call makefiles libs
	@for i in $(NEED_MAKE); do \
		make -C $$i; \
	done
	$(eval NEED_MAKE := )

# if OBJS_DIR && (((DEBUG && !debugFile) || (!DEBUG && debugFile))) then fclean
	@if [ -d $(OBJS_DIR) ] && \
(([ ! -z $(DEBUG) ] && [ -d $(DEBUG_DIR) ] && [ ! -f $(DEBUG_DIR)/DEBUG ]) || \
([ -z $(DEBUG) ] && [ -d $(DEBUG_DIR) ] && [ -f $(DEBUG_DIR)/DEBUG ])); then\
		$(MAKE) $(MAKE_OPT) fclean NEED_MAKE=$(NEED_MAKE);\
	fi;

	$(START)
	@$(MAKE) $(MAKE_OPT) $(NAME)
	$(END)

ifneq ($(DEBUG),)
	@touch $(DEBUG_DIR)/DEBUG
else
	@rm -f $(DEBUG_DIR)/DEBUG
endif

configure:
	@make install
	@make create_dmg

install:
	@for i in $(NEED_MAKE); do \
		make -C $$i install; \
	done
	$(eval NEED_MAKE := )
	@printf $(YELLOW)$(BOLD)"INSTALL $(PROJECT_NAME)\n--------------------\n"$(NORMAL)
	@printf $(CYAN)"-> init submodules\n"$(NORMAL)
	@git submodule update --init
	@printf $(CYAN)"-> install $(NAME)\n"$(NORMAL)
	@echo "$$CONFIGURE" > .tmpfile.sh
	@chmod 755 .tmpfile.sh
	@./.tmpfile.sh
	@rm -f ./.tmpfile.sh
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)
	@$(MAKE) $(MAKE_OPT) install_linter
	@$(MAKE) $(MAKE_OPT) init

uninstall:
	@printf $(YELLOW)$(BOLD)"DELETE $(PROJECT_NAME) FILES\n--------------------\n"$(NORMAL)
ifeq ($(UNAME_S),Linux)
	@printf $(RED)"-> nothing to do in this section for linux.\n"$(NORMAL)
endif
ifeq ($(UNAME_S),Darwin)
	@printf $(RED)"-x remove /Application/$(NAME).app\n"$(NORMAL)
	@rm -rf /Application/$(NAME).app
	@printf $(RED)"-x remove settings and saves of the app on $(HOME)/Library/Application\ Support/bomberman\n"$(NORMAL)
	@rm -rf $(HOME)/Library/Application\ Support/bomberman
endif
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)

install_linter:
	@printf $(YELLOW)$(BOLD)"INSTALL LINTER\n--------------------\n"$(NORMAL)
	@printf $(CYAN)"-> install linter\n"$(NORMAL)
	@echo "$$CONFIGURE_LINTER" > .tmpfile.sh
	@chmod 755 .tmpfile.sh
	@./.tmpfile.sh
	@rm -f ./.tmpfile.sh
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)

create_dmg: all
	@printf $(YELLOW)$(BOLD)"CREATE INSTALLATION FILE\n--------------------\n"$(NORMAL)
ifeq ($(UNAME_S),Linux)
	@printf $(RED)"-> unable to create a linux installer\n"$(NORMAL)
endif
ifeq ($(UNAME_S),Darwin)
	@printf $(CYAN)"-> create ~/Downloads/$(NAME).dmg\n"$(NORMAL)
	@./scripts/createApp.sh ~/Downloads
endif
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)

init:
	@printf $(YELLOW)$(BOLD)"INIT $(PROJECT_NAME)\n--------------------\n"$(NORMAL)
	@printf $(CYAN)"-> create pre-commit\n"$(NORMAL)
	@echo "$$PRE_COMMIT" > $(PRE_COMMIT_FILE)
	@chmod 755 $(PRE_COMMIT_FILE)
	@printf $(CYAN)"-> create pre-push\n"$(NORMAL)
	@echo "$$PRE_PUSH" > $(PRE_PUSH_FILE)
	@chmod 755 $(PRE_PUSH_FILE)
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)

$(NAME): $(OBJS_DIR) $(OBJS)
	@printf $(CYAN)"-> create program : $(NAME)\n"$(NORMAL)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS_FLAGS) -ldl

$(OBJS_DIR)/%.o: $(LIBS_DIR)/%.c
$(OBJS_DIR)/%.o: $(LIBS_DIR)/%.c $(DEP_DIR)/%.d
	@printf $(YELLOW)"-> $<\n"$(NORMAL)
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@ $(INC)
	@mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d


$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(DEP_DIR)/%.d
	@printf $(YELLOW)"-> $<\n"$(NORMAL)
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@ $(INC)
	@mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

$(OBJS_DIR):
	@mkdir -p $(dir $(OBJS))

$(DEP_DIR):
	@mkdir -p $(dir $(DEPFILES))

$(DEP_DIR)/%.d: $(DEP_DIR) ;

-include $(DEPFILES)

clean:
	@for i in $(NEED_MAKE); do \
		make -C $$i clean; \
	done
	$(eval NEED_MAKE := )
	$(START)
	@printf $(RED)"-x remove .o & .d files\n"$(NORMAL)
	@rm -rf $(OBJS_DIR)
	@rm -rf $(DEP_DIR)
	@rm -rf $(DEBUG_DIR)
	@rm -rf libs/ANibblerGui.o
	@rm -rf libs/ANibblerGui.d
	$(END)

fclean:
	@for i in $(NEED_MAKE); do \
		make -C $$i fclean; \
	done
	$(eval NEED_MAKE := )
	@$(MAKE) $(MAKE_OPT) clean NEED_MAKE=$(NEED_MAKE)
	$(START)
	@printf $(RED)"-x remove $(NAME)\n"$(NORMAL)
	@rm -f $(NAME)
	$(END)

re:
	@for i in $(NEED_MAKE); do \
		make -C $$i re; \
	done
	$(eval NEED_MAKE := )
	@$(MAKE) $(MAKE_OPT) fclean NEED_MAKE=$(NEED_MAKE)
	@$(MAKE) $(MAKE_OPT) NEED_MAKE=$(NEED_MAKE)

exec-nolint:
	@$(MAKE) $(MAKE_OPT)
	@printf $(MAGENTA)$(BOLD)"EXEC $(PROJECT_NAME)\n--------------------\n"$(NORMAL)
	@./$(NAME) $(ARGS) || \
	        (printf $(MAGENTA)$(BOLD)"\n--------------------\n"$(RED)"❌ ERROR: $$?\n"$(NORMAL))
	@printf $(MAGENTA)$(BOLD)"--------------------\n"$(NORMAL)

exec:
	@$(MAKE) $(MAKE_OPT) lint ; true
	@$(MAKE) $(MAKE_OPT) exec-nolint ; true

lint:
	@for i in $(NEED_MAKE); do \
		make -C $$i lint; \
	done
	@printf $(BLUE)$(BOLD)"LINTER ON $(PROJECT_NAME)\n--------------------\n"$(NORMAL)
	@if [ "$(LINTER)" = "" ]; then\
		printf $(RED)$(BOLD)"Error:"$(NORMAL)" env var CPPLINT is not set\n"; \
	else \
		$(LINTER) $(LINTER_RULES) $(addprefix $(INC_DIR)/, $(HEAD)) $(addprefix $(SRCS_DIR)/, $(SRC)); \
    fi
	@printf $(BLUE)$(BOLD)"--------------------\n"$(NORMAL)

check:
	@for i in $(NEED_MAKE); do \
		make -C $$i check; \
	done
	$(eval NEED_MAKE := )
	@$(MAKE) $(MAKE_OPT) fclean NEED_MAKE=$(NEED_MAKE)
	@$(MAKE) $(MAKE_OPT) lint NEED_MAKE=$(NEED_MAKE)
	@$(MAKE) $(MAKE_OPT) doc NEED_MAKE=$(NEED_MAKE)
	@$(MAKE) $(MAKE_OPT) NEED_MAKE=$(NEED_MAKE)

doc:
	@printf $(BLUE)$(BOLD)"CHECKING DOCUMENTATION FOR $(PROJECT_NAME)\n--------------------\n"$(NORMAL)
	@doxygen scripts/Doxyfile
	@printf $(YELLOW)"all warnings:\n"$(NORMAL)
	@cat /tmp/doxygen_warn.log
	@printf $(BLUE)$(BOLD)"--------------------\n"$(NORMAL)
	@if [ ! "`cat /tmp/doxygen_warn.log`" = "" ]; then \
		exit 1; \
	fi

ghpages:
	@printf $(YELLOW)$(BOLD)"UPDATE DOC ON GITHUB PAGES FOR $(PROJECT_NAME)\n--------------------\n"$(NORMAL)
	@printf $(CYAN)"-> update github pages\n"$(NORMAL)
	@./scripts/updateGithubPages.sh
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)

info:
	@printf $(YELLOW)$(BOLD)"INFO FOR $(PROJECT_NAME)\n--------------------\n"$(NORMAL)
	@printf "`ls -1 srcs/**/*.cpp includes/**/*.hpp shaders/**/*.glsl | wc -l | sed 's/ //g'` code files:\n"
	@printf "\t`ls -1 srcs/**/*.cpp | wc -l | sed 's/ //g'` cpp in srcs/\n"
	@printf "\t`ls -1 includes/**/*.hpp | wc -l | sed 's/ //g'` hpp in includes/\n"
	@printf "\t`ls -1 shaders/**/*.glsl | wc -l | sed 's/ //g'` glsl in shaders/\n"
	@printf "`cat srcs/**/*.cpp includes/**/*.hpp shaders/**/*.glsl | wc -l | sed 's/ //g'` code lines:\n"
	@printf "\t`cat srcs/**/*.cpp | wc -l | sed 's/ //g'` cpp lines in srcs/\n"
	@printf "\t`cat includes/**/*.hpp | wc -l | sed 's/ //g'` hpp lines in includes/\n"
	@printf "\t`cat shaders/**/*.glsl | wc -l | sed 's/ //g'` glsl lines in shaders/\n"
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)

help:
	@printf $(YELLOW)$(BOLD)"HELP\n--------------------\n"$(NORMAL)
	@printf $(NORMAL)"-> make "$(BOLD)"configure"$(NORMAL)": install all, compile & create the .dmg file in ~/Downloads\n"
	@printf $(NORMAL)"-> make "$(BOLD)"install"$(NORMAL)": install all depencies + linter & run make init\n"
	@printf $(NORMAL)"-> make "$(BOLD)"create_dmg"$(NORMAL)": create $(NAME).dmg file for installation\n"
	@printf $(NORMAL)"-> make "$(BOLD)"uninstall"$(NORMAL)": uninstall app (if installed)\n"
	@printf $(NORMAL)"-> make "$(BOLD)"install_linter"$(NORMAL)": install the linter\n"
	@printf $(NORMAL)"-> make "$(BOLD)"init"$(NORMAL)": init the project (add pre-commit & pre-push files)\n"
	@printf $(NORMAL)"-> make "$(BOLD)"all"$(NORMAL)": build the project and create $(NAME)\n"
	@printf $(NORMAL)"-> make "$(BOLD)"clean"$(NORMAL)": remove all .o files\n"
	@printf $(NORMAL)"-> make "$(BOLD)"fclean"$(NORMAL)": make clean and remove executable\n"
	@printf $(NORMAL)"-> make "$(BOLD)"re"$(NORMAL)": make fclean and make all\n"
	@printf $(NORMAL)"-> make "$(BOLD)"lint"$(NORMAL)": exec linter on project (use env var CPPLINT)\n"
	@printf $(NORMAL)"-> make "$(BOLD)"exec"$(NORMAL)": make lint, make all and exec the program: ./$(NAME) ARGS('$(ARGS)')\n"
	@printf $(NORMAL)"-> make "$(BOLD)"exec-nolint"$(NORMAL)": make all and exec the program: ./$(NAME) ARGS('$(ARGS)')\n"
	@printf $(NORMAL)"-> make "$(BOLD)"check"$(NORMAL)": make fclean, make lint, make exec-nolint -> stop if there is an error\n"
	@printf $(NORMAL)"-> make "$(BOLD)"doc"$(NORMAL)": check all warnings on documentation\n"
	@printf $(NORMAL)"-> make "$(BOLD)"ghpages"$(NORMAL)": generate the doc on gh-pages branch\n"
	@printf $(NORMAL)"-> make "$(BOLD)"info"$(NORMAL)": display informations about the program\n"
	@printf $(NORMAL)"-> make "$(BOLD)"help | usage"$(NORMAL)": show the help\n"
	@printf $(NORMAL)"-> make "$(BOLD)"... DEBUG=1"$(NORMAL)": use debug mode\n"
	@printf $(NORMAL)"-> make "$(BOLD)"... ARGS='...'"$(NORMAL)": add arguments to exec: ./$(NAME) ARGS (for make exec & exec-nolint)\n"
	@printf $(NORMAL)"-> make "$(BOLD)"... DEFINE=\"DEFINE=value\""$(NORMAL)": send `#define DEFINE value` to all files\n"
	@printf $(YELLOW)$(BOLD)"--------------------\n"$(NORMAL)

usage: help

.PHONY: configure install create_dmg uinstall install_linter init all clean fclean re exec-nolint exec lint check doc ghpages info help usage
