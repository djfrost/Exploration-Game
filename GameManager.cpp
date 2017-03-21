#include "GameManager.h"
#include "RenderManager.h"
#include "LevelLoader.h"
#include "LogManager.h"
#include "InputManager.h"
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
	logManager = new LogManager("GameLog.txt");
	logComment("Loading Render Manager");
	render_manager = new RenderManager(this);  //calls render manager's init, sets up the frame listener
	logComment("Render Manager loaded successfully");
	inputManager = new InputManager(this);
	logComment("Input Manager loaded successfully");
	std::string file = "resources.json";
	levelLoader = new LevelLoader(this, file);
	logComment("Level Loader loaded successfully");
}

GameManager::GameManager(){
	init();
	render_manager->startRendering();
}

GameManager::~GameManager(){
	std::cout << "GameManager destructor called" << std::endl;
	delete levelLoader;
	std::cout << "Deleted Level Loader" << std::endl;
	delete inputManager;
	std::cout << "Deleted Input Manager" << std::endl;
	delete logManager;
	std::cout << "Deleted Log Manager" << std::endl;
	delete render_manager;
	std::cout << "Deleted Render Manager" << std::endl;
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

void GameManager::loadScene(std::string scene, std::string lastScene, std::vector<std::string> meshNames, std::vector<std::string> meshFiles, std::vector < std::vector<float> > transforms, std::vector < std::vector<float> > rotates, std::vector<float> angle, std::vector < std::vector<float> > scales, std::vector<std::string> animNames ){
	render_manager->loadScene(scene, lastScene, meshNames, meshFiles, transforms, rotates, angle, scales, animNames);
}
void GameManager::loadCameras(std::vector< std::vector< float > > positions, std::vector< std::vector < float > > lookAts, std::vector<float> nearclips, std::vector<float> farclips, std::vector<float> rotation, std::vector<float> angle, std::vector<std::string> parents){
	render_manager->loadCameras(positions, lookAts, nearclips, farclips, rotation, angle, parents);
}
void GameManager::loadLights(std::vector<std::string> names, std::vector<float> types, std::vector< std::vector < float > > colors, std::vector< std::vector < float > > directions){
	render_manager->loadLights(names, types, colors, directions);
}
void GameManager::loadSkyBox(std::string skyBoxMat){
	render_manager->loadSkyBox(skyBoxMat);
}
void GameManager::processAnims(std::vector<std::string> objects, std::vector<std::string> types, std::vector< std::vector < float > > values, std::vector< std::vector < float > >  axis, std::vector< std::vector < float > > timeSteps, std::vector< std::vector < float > > start, std::vector < std::vector<float> > begin){
	render_manager->processAnims(objects, types, values, axis, timeSteps, start, begin);
}
void GameManager::initialiseNewScene(){
	render_manager->initialiseNewScene();
}
void GameManager::logComment(std::string comment){
	logManager->logComment(comment);
}
void GameManager::checkForInput(float time_step){
	inputManager->checkForInput();
}
void GameManager::keyPressed(std::string keyPressed){
	render_manager->processKeyboardInput(keyPressed, false);
}
bool GameManager::keyReleased(std::string keyUp){
	render_manager->processKeyboardInput(keyUp, true);
}
void GameManager::logProblem(std::string prob, std::string file, int line){
	logManager->logProblem(prob, file, line);
}
void GameManager::leftJoystickAxisMoved(float north_south, float east_west){
	render_manager->leftJoystickAxisMoved(north_south, east_west);
}
void GameManager::rightJoystickAxisMoved(float north_south, float east_west){
	render_manager->rightJoystickAxisMoved(north_south, east_west);
}