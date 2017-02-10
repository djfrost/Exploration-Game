#include "LevelLoader.h"
#include <boost/foreach.hpp>
#include "GameManager.h"
LevelLoader::LevelLoader(GameManager* gameManager, std::string fileName){
	gm = gameManager;
	ReadResources(fileName);
	//Now that we have the ptree from the json file, we want to load the first level
	std::string levelOne = pt.get<std::string>("first_level");
	currScene = levelOne;
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

void LevelLoader::LoadLevel(std::string levelName)
{
	nextScene = levelName;
	ptree levelTree;
	
	std::string level = "levels." + levelName;
	levelTree = pt.get_child(level);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, levelTree.get_child("path")){
		gm->addPath(v.second.data(), levelName);
	}
	ptree meshTree = levelTree.get_child("meshes");
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, meshTree.get_child("mesh")){
		ptree meshEntityTree = meshTree.get_child(v.second.data());
		std::string meshFile;
		for(auto& fileItem : meshEntityTree.get_child("file")){
			meshFile = fileItem.second.get_value<std::string>();
		}
	
		std::vector<float> transform;
		for (auto& item : meshEntityTree.get_child("transform")){
			transform.push_back(item.second.get_value<float>());
		}
		std::vector<float> rotate;
		for (auto& item : meshEntityTree.get_child("rotate")){
			rotate.push_back(item.second.get_value<float>());
		}std::vector<float> scale;
		for (auto& item : meshEntityTree.get_child("scale")){
			scale.push_back(item.second.get_value<float>());
		}
		gm->addMesh(meshFile, transform, rotate, scale, levelName);
	}
	// unload last scene load next scene
	gm->loadScene(levelName);
}
/*std::vector<float> LevelLoader::parse3F(std::string floats){
	std::vector<float> floatVec;
	int firstDelim = floats.find(",");
	floatVec.push_back(stof(floats.substr(0,firstDelim)));
	floats.erase(0, firstDelim);
	firstDelim = floats.find(",");
	floatVec.push_back(stof(floats.substr(0,firstDelim)));
	floats.erase(0, firstDelim);
	floatVec.push_back(stof(floats.substr(0,floats.length)));
}*/