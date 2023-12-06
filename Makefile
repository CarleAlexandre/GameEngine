NAME		=	noHeaven

SERVER_NAME	=	server

BUILDDIR	=	build/

CC			=	gcc

CFLAGS		=	-Wall -Wextra -std=c++20 -O3

OBJ		=	$(SRC:%.cc=%.o)

SERVER_OBJ	=	$(SERVER_SRC:%.cc=%.o)

DEPS	+=	$(wildcard include/*.hh)

SRC		+=	$(wildcard math/*.cc)
SRC		+=	$(wildcard render/*.cc)
SRC		+=	$(wildcard ui/*.cc)
SRC		+=	$(wildcard io/*.cc)
SRC		+=	$(wildcard game/*.cc)
SRC		+=	$(wildcard physic/*.cc)
SRC		+=	$(wildcard console/*.cc)

SERVER_SRC	+=	source/server.cpp

#$(wildcard ./source/*.cpp)

INCLUDE		=	-I C:/mingw64/include

ifeq ($(OS), Windows_NT)
LIBS		=	-lraylib -lopengl32 -lgdi32 -lwinmm -lstdc++
endif
ifeq ($(shell uname -s), Linux)
LIBS		=	-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
endif

$(BUILDDIR)$(NAME)		:	$(OBJ) $(DEPS)
		mkdir -p $(BUILDDIR)
		$(CC) $(OBJ) ${LIBS} -g -o $@

#add this above after lib to have no console: <-mwindows -Wl,--subsystem,windows>

$(OBJ)		:	%.o	:	%.cc $(DEPS)
		$(CC) $(CFLAGS) ${INCLUDE} -g -c $< -o $@

#$(SERVER_NAME)	:	$(OBJ)
#		$(CC) $(SERVER_OBJ) ${SERVER_LIBS} -o $@


#$(SERVER_OBJ)	:	%.o	:	%.cpp
#		$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

all		:	$(BUILDDIR)$(NAME)

craft	:
	g++ JsonCraftGenerator/main.cpp source/math.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -lstdc++ -I C:/mingw64/include -Wall -Wextra -std=c++20 -O3

clean		:
		rm -rf $(OBJ)

fclean		:	clean
		rm -rf $(BUILDDIR)

re		:	fclean all

