#include "GameManager.h"
#include "RenderManager.h"
#include "LevelLoader.h"
#include <iostream>
#include <string>
//the problem is that in C++, it is not known when this initialization code will be called
//so we will use a function static variable that is initialized the first time the method is called
//destructor automatically called?
GameManager* GameManager::getGameManager(){
	static GameManager game_manager;  //initialized when method is called the first time
	return &game_manager;  //won't go out of scope as game_manager is static
}

void GameManager::init(){
	render_manager = new RenderManager(this);  //calls render manager's init, sets up the frame listener
	std::string file = "resources.json";
	levelLoader = new LevelLoader(this, file);
}

GameManager::GameManager(){
	init();
	render_manager->startRendering();
}

GameManager::~GameManager(){
	std::cout << "GameManager destructor called" << std::endl;

	delete render_manager;
	render_manager = NULL;
}

size_t GameManager::getRenderWindowHandle(){
	return render_manager->getRenderWindowHandle();
}

int GameManager::getRenderWindowWidth(){
	return render_manager->getRenderWindowWidth();
}

int GameManager::getRenderWindowHeight(){
	return render_manager->getRenderWindowHeight();
}

void GameManager::addPath(std::string path, std::string resourceGroup){
	render_manager->addPath(path, resourceGroup);
}
void GameManager::addMesh(std::string mesh, std::vector<float> transform, std::vector<float> rotate, std::vector<float> scale, std::string resourceGroup, std::string mesh_file){
	render_manager->addMesh(mesh, resourceGroup, mesh_file);
}

void GameManager::loadScene(std::string scene, std::string lastScene, std::vector<std::string> meshNames, std::vector<std::string> meshFiles, std::vector < std::vector<float> > transforms, std::vector < std::vector<float> > rotates, std::vector<float> angle, std::vector < std::vector<float> > scales ){
	render_manager->loadScene(scene, lastScene, meshNames, meshFiles, transforms, rotates, angle, scales);
}
void GameManager::loadCameras(std::vector< std::vector< float > > positions, std::vector< std::vector < float > > lookAts, std::vector<float> nearclips, std::vector<float> farclips){
	render_manager->loadCameras(positions, lookAts, nearclips, farclips);
}
void GameManager::loadLights(std::vector<std::string> names, std::vector<float> types, std::vector< std::vector < float > > colors, std::vector< std::vector < float > > directions){
	render_manager->loadLights(names, types, colors, directions);
}
void GameManager::loadSkyBox(std::string skyBoxMat){
	render_manager->loadSkyBox(skyBoxMat);
}
void GameManager::initialiseNewScene(){
	render_manager->initialiseNewScene();
}