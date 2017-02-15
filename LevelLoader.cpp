#include "LevelLoader.h"
#include <boost/foreach.hpp>
#include "GameManager.h"
#include <sstream>
LevelLoader::LevelLoader(GameManager* gameManager, std::string fileName){
	gm = gameManager;
	ReadResources(fileName);
	//Now that we have the ptree from the json file, we want to load the first level
	std::string levelOne = pt.get<std::string>("first_level");
	currScene = "";
	nextScene = "";
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
	gm->initialiseNewScene();
	std::ofstream outFile("LevelLoader.out");
	outFile << levelName;
	std::string level = "levels." + levelName;
	levelTree = pt.get_child(level);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, levelTree.get_child("path")){
		gm->addPath(v.second.data(), levelName);
	}
	ptree meshTree = levelTree.get_child("meshes");
	std::vector<std::string> meshes;
	std::vector<std::string> meshFiles;
	std::vector< std::vector< float > > transforms;
	std::vector< std::vector< float > > rotates;
	std::vector< std::vector< float > > scales;
	std::vector<float> angle;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, meshTree.get_child("meshNames")){
		outFile << v.second.data();
		BOOST_FOREACH(boost::property_tree::ptree::value_type &t, meshTree.get_child(v.second.data())){
			std::string type = t.first.data();
			if(type == "file")
			{
				outFile << t.second.data();
				meshFiles.push_back(t.second.data());
			}
			else if(type == "transform"){
				std::string transNotParsed = t.second.data();
				transforms.push_back(parse3F(transNotParsed));
			}
			else if(type == "rotate"){
				std::string rotNotParsed = t.second.data();
				rotates.push_back(parse3F(rotNotParsed));
			}
			else if(type == "scale"){
				std::string scaleNotParsed = t.second.data();
				scales.push_back(parse3F(scaleNotParsed));
			}
			else if(type == "angle"){
				std::string anglestr = t.second.data();
				angle.push_back(stof(anglestr));
			}
		}
		meshes.push_back(v.second.data());
		//gm->addMesh(v.second.data(), transform, rotate, scale, levelName, meshFile);
	}
	// unload last scene load next scene
	gm->loadScene(nextScene, currScene, meshes, meshFiles, transforms, rotates, angle, scales);
	currScene = levelName;
	nextScene = "";
}
std::vector<float> LevelLoader::parse3F(std::string floats){
	std::vector<float> floatVec;
	std::stringstream ss;
	ss.str(floats);
	std::string tempFloat;
	while(std::getline(ss, tempFloat, ',')){
		floatVec.push_back(stof(tempFloat));
	}
	return floatVec;
}