NAME_EXEC		= nibbler
SRC_DIR			= src
SRC_EXE			= $(SRC_DIR)/main.cpp \
			  $(SRC_DIR)/Game.cpp \
			  $(SRC_DIR)/Fruit.cpp \
			  $(SRC_DIR)/Snake.cpp \
			  $(SRC_DIR)/Exeption.cpp \
			  $(SRC_DIR)/DynLib.cpp
OBJ_EXE			= $(SRC_EXE:.cpp=.o)

NAME_LIB_SDL		= lib_nibbler_sdl.so
SRC_LIB_SDL		= $(SRC_DIR)/NibblerSDL.cpp
FLAG_SDL		= -fPIC -shared -lSDL -lSDL_image -lSDL_ttf 


NAME_LIB_NCURSES	= lib_nibbler_ncurses.so
SRC_LIB_NCURSES		= $(SRC_DIR)/NibblerNCurses.cpp
FLAG_NCURSES		= -fPIC -shared -lncurses

NAME_LIB_OPENGL		= lib_nibbler_opengl.so
SRC_LIB_OPENGL		= $(SRC_DIR)/NibblerOpenGL.cpp
FLAG_OPENGL		= -fPIC -shared  -lGL -lGLU -lSDL -lSDL_image

CXXFLAGS		+= -W
CXXFLAGS		+= -Wall
CXXFLAGS		+= -Wextra
CXXFLAGS		+= -ansi
CXXFLAGS		+= -fPIC
CXXFLAGS		+= -Iinc

CXX 			= g++

all			: $(NAME_EXEC)
all			: $(NAME_LIB_SDL)
all			: $(NAME_LIB_OPENGL)
all			: $(NAME_LIB_NCURSES)

$(NAME_EXEC)		: $(OBJ_EXE)
			$(CXX) -o $(NAME_EXEC) $(OBJ_EXE) -ldl

$(NAME_LIB_SDL)		:
			$(CXX) $(CXXFLAGS) $(FLAG_SDL) -o $(NAME_LIB_SDL) $(SRC_LIB_SDL)

$(NAME_LIB_NCURSES)	:
			$(CXX) $(CXXFLAGS) $(FLAG_NCURSES) -o $(NAME_LIB_NCURSES) $(SRC_LIB_NCURSES)

$(NAME_LIB_OPENGL)	:
			$(CXX) $(CXXFLAGS) $(FLAG_OPENGL) -o $(NAME_LIB_OPENGL) $(SRC_LIB_OPENGL)

clean			:
			$(RM) $(OBJ_LIB_SDL) $(OBJ_LIB_OPENGL) $(OBJ_LIB_NCURSES) $(OBJ_EXE)

fclean			: clean
			$(RM) $(NAME_LIB_SDL) $(NAME_LIB_OPENGL) $(NAME_LIB_NCURSES) $(NAME_EXEC)

re			: fclean all

d			: CXXFLAGS += -ggdb3
d			: fclean all

.PHONY			: all clean fclean re d
.PRECIOUS		: $(SRC_LIB_SDL) $(SRC_LIB_OPENGL) $(SRC_LIB_NCURSES) $(SRC_EXE)
.DEFAULT		:
	@echo "Wrong option : $@."
