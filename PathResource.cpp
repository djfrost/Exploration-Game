#include "PathResource.h"
#include "GameManager.h"

PathResource::PathResource(std::string level_name, std::string path, GameResourceType type, GameManager* game_manager) : GameResource(level_name, path, type){
	gm = game_manager;
}
PathResource::~PathResource(){
	gm = NULL;
}
void PathResource::load(){
	gm->addPath(getResourceFileName(), getResourceLevelName());
	is_loaded = true;
}
void PathResource::unload(){
	is_loaded = false;
}