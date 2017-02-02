AutomatedMakefile = am
CC = g++ -std=c++11

INC_DIRS=-I./ -I$(OGRE_PATH)/OgreMain/include -I$(BOOST_PATH)
LIB_DIRS=-L./ -L$(OGRE_PATH)/build/lib -L$(BOOST_PATH)/stage/lib
LIBS=-lboost_system-mgw51-mt-1_63 -lOgreMain 

COMPILE = $(CC) $(INC_DIRS) -c
LINK = $(CC) $(LIB_DIRS) -o

FILES =  LevelLoader.o RenderManager.o GameManager.o GameDriver.o
all: Ogre

Ogre: 		$(FILES)
		$(LINK)  Game.exe $(FILES) $(LIBS)

GameManager.o: 	GameManager.h GameManager.cpp
			$(COMPILE) GameManager.cpp
LevelLoader.o:	LevelLoader.h LevelLoader.cpp
			$(COMPILE) LevelLoader.cpp
RenderManager.o: RenderManager.h RenderManager.cpp
			$(COMPILE) RenderManager.cpp
GameDriver.o:	GameDriver.cpp
			$(COMPILE) GameDriver.cpp