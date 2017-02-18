#if !defined LEVEL_LOADER
#define LEVEL_LOADER

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include <iostream>
#include <string>
#include <vector>
using boost::property_tree::ptree;

class GameManager;
/*
	LevelLoader is intended to load the json fileName
	that holds the game resources, and parse it. It is
	also responsible for loading levels.
*/
class LevelLoader{
	private:
		//Reference to GameManager which holds a reference to all of our other components
		GameManager* gm;
		//Stores the json representation so we only have to read the file once
		ptree pt;
		//Loads the json file into pt
		void ReadResources(std::string fileName);
		std::string currScene;
		std::string nextScene;
		std::vector<float> parseMultF(std::string floats);
	public: 
		//Loads the json file, and stores a reference to the game manager
		LevelLoader(GameManager* gameManager, std::string fileName);
		//Deletes references
		~LevelLoader();
		//Loads a level given it's name
		void LoadLevel(std::string levelName);
};
#endif