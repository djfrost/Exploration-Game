#include "LevelLoader.h"
#include <vector>
#include <boost/foreach.hpp>
LevelLoader::LevelLoader(GameManager* gameManager, std::string fileName){
	gm = gameManager;
	ReadResources(fileName);
	//Now that we have the ptree from the json file, we want to load the first level
	std::string levelOne = pt.get<std::string>("first_level");
	LoadLevel(levelOne);
}

LevelLoader::~LevelLoader(){
	
}

void LevelLoader::ReadResources(std::string fileName){
	std::ifstream jsonFile(fileName.c_str());
    read_json(jsonFile, pt);
	std::string output;
	jsonFile.close();
}

int LevelLoader::GetNumLevels(){
	
}

void LevelLoader::LoadLevel(std::string levelName){
	ptree levelTree;
	std::ofstream outFile("LevelLoader.out");
	outFile << levelName;
	
	std::string level = "levels." + levelName;
	levelTree = pt.get_child(level);
	//get path
	std::vector< std::string > paths;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, levelTree.get_child("path")){
		assert(v.first.empty()); // array elements have no names
		paths.push_back(v.second.data());
		outFile << "\t" + v.second.data();
	}
	outFile.close();
	//get meshes
}