#include "LevelLoader.h"
#include <vector>
#include <boost/foreach.hpp>
#include "GameManager.h"
#include <typeinfo>
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
		AddPath(v.second.data());
	}
	ptree meshTree = levelTree.get_child("meshes");
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, meshTree.get_child("")){
		LoadMesh(v.second.data());
	}
}

/*
	Given a mesh file as a string, this function will
	load the mesh and send it to the next scene. When the
	next scene is loaded, this mesh will be inside of it.
*/
void LevelLoader::LoadMesh(std::string mesh){
	
}

/*
	Given a path as a string, it adds it to the resource
	group manager for the next level.
*/
void LevelLoader::AddPath(std::string path){
	
}