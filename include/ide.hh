#ifndef ENGINE_HH
# define ENGINE_HH

# ifdef _WIN64
#  include <C:/mingw64/include/raylib.h>
#  include <C:/mingw64/include/raymath.h>
#  include <C:/mingw64/include/rlgl.h>
#  define RLIGHTS_IMPLEMENTATION
#  include "rlights.h"
#  define GLSL_VERSION 330
# else
#  include <raylib.h>
#  include <raymath.h>
#  include <rlgl.h>
#  define RLIGHTS_IMPLEMENTATION
#  include "rlights.h"
#  define GLSL_VERSION 330
#  include <cstring>
# endif

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <math.h>
# include <iostream>
# include <fstream>
# include <vector>
# include <sstream>
# include <string>

enum engine_status {
	s_menu = 0,
	s_game = 1,
	s_close = 2,
	s_setting = 3,
	s_pause = 4,
	s_gameover = 5,
	s_debug = 6,
};

typedef struct s_display {
	unsigned short	width;
	unsigned short	height;
	bool			fullscreen;
	unsigned int	configflag;
	char			*title;
	void			*handle;
	//Image			icon;
}	display_t;

typedef struct s_engine {
	int	status;
}	engine_t;


#endif