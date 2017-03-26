AutomatedMakefile = am
CC = g++ -std=c++11

INC_DIRS=-I./ -I$(OGRE_PATH)/OgreMain/include -I$(BOOST_PATH) -I$(OIS_PATH)/include -I$(LUA_PATH)/src -I$(LUA_PATH)/luawrapper
LIB_DIRS=-L./ -L$(OGRE_PATH)/build/lib -L$(BOOST_PATH)/stage/lib -L$(OIS_PATH)/lib -L$(LUA_PATH)/lib
LIBS=-lboost_system-mgw51-mt-1_63 -lois -llua -lOgreMain 

COMPILE = $(CC) $(INC_DIRS) -c
LINK = $(CC) $(LIB_DIRS) -o

FILES = InputRenderListener.o InputManager.o LevelLoader.o AnimationRenderListener.o RenderManager.o LogManager.o GameManager.o GameDriver.o RenderListener.o InputFunctionHandler.o
all: Ogre

Ogre: 		$(FILES)
			$(LINK)  Game.exe $(FILES) $(LIBS)
InputFunctionHandler.o: InputFunctionHandler.h InputFunctionHandler.cpp
			$(COMPILE) InputFunctionHandler.cpp
RenderListener.o: RenderListener.h RenderListener.cpp
			$(COMPILE) RenderListener.cpp
InputRenderListener.o: InputRenderListener.h InputRenderListener.cpp
			$(COMPILE) InputRenderListener.cpp
InputManager.o : InputManager.h InputManager.cpp
			$(COMPILE) InputManager.cpp
GameManager.o: 	GameManager.h GameManager.cpp
			$(COMPILE) GameManager.cpp
LevelLoader.o:	LevelLoader.h LevelLoader.cpp
			$(COMPILE) LevelLoader.cpp
RenderManager.o: RenderManager.h RenderManager.cpp
			$(COMPILE) RenderManager.cpp
LogManager.o:	LogManager.h LogManager.cpp
			$(COMPILE) LogManager.cpp
GameDriver.o:	GameDriver.cpp
			$(COMPILE) GameDriver.cpp
AnimationRenderListener.o: AnimationRenderListener.h AnimationRenderListener.cpp
			$(COMPILE) AnimationRenderListener.cpp