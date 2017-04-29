#include "GameManager.h"
#include "RenderManager.h"
#include "LevelLoader.h"
#include "LogManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "GUIManager.h"
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
	audioManager = new AudioManager(this);
	logComment("AudioManager Loaded successfully");
	guiManager = new GUIManager(render_manager);
	logComment("GUI Manager loaded successfully");
	scriptManager = new ScriptManager(this);
	logComment("Script Manager loaded successfully");
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
	delete scriptManager;
	std::cout << "Deleted Script Manager" << std::endl;
	delete audioManager;
	std::cout << "Deleted Audio Manager" << std::endl;
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
void GameManager::loadKeyScripts(std::vector<std::string> scriptKeys, std::vector<std::string> keyScripts){
	inputManager->loadKeyScripts(scriptKeys, keyScripts);
}
void GameManager::checkForInput(float time_step){
	inputManager->checkForInput();
}
void GameManager::keyPressed(std::string keyPressed){
	render_manager->processKeyboardInput(keyPressed, false);
	guiManager->keyPressed(keyPressed);
}
void GameManager::keyPressed(std::string keyPressed, std::string script){
	render_manager->processKeyboardInput(keyPressed, false);
	scriptManager->executeScript(script);
	guiManager->keyPressed(keyPressed);
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
void GameManager::loadSampleAudioResource(std::string audio_file_name, AudioResourceInfo* ar_info){
	audioManager->loadSampleAudioResource(audio_file_name, ar_info);
}
void GameManager::loadStreamAudioResource(std::string audio_file_name, AudioResourceInfo* ar_info){
	audioManager->loadStreamAudioResource(audio_file_name, ar_info);
}
void GameManager::unloadSampleAudioResource(AudioResourceInfo* ar_info){
	audioManager->unloadSampleAudioResource(ar_info);
}
void GameManager::unloadStreamAudioResource(AudioResourceInfo* ar_info){
	audioManager->unloadStreamAudioResource(ar_info);
}
void GameManager::playAudio(AudioResource* ar, int num_repeats){
	audioManager->playAudio(ar, num_repeats);
}
void GameManager::updateAudio(float time_step){
	audioManager->updateAudio(time_step);
}
AudioResourceInfo* GameManager::createAudioResourceInfo(){
	return audioManager->createAudioResourceInfo();
}
void GameManager::mousePressed(unsigned int x_click,unsigned int y_click, int mouseButton){
	render_manager->mousePressed(x_click, y_click, mouseButton);
	guiManager->mousePressed(x_click, y_click, mouseButton);
}
void GameManager::mouseReleased(unsigned int x_click,unsigned int y_click, int mouseButton){
	render_manager->mouseReleased(x_click, y_click, mouseButton);
}
void GameManager::mouseMoved(unsigned int x_click,unsigned int y_click,float x_rel,float y_rel){
	render_manager->mouseMoved(x_click, y_click, x_rel, y_rel);
	guiManager->mouseMoved(x_click, y_click, x_rel, y_rel);
}
void GameManager::guiLoadLevel(std::string level_name, std::string scheme, std::string font, std::string cursor, std::string tooltip, std::string layout){
	guiManager->loadLevel(level_name, scheme, font, cursor, tooltip, layout);
}
void GameManager::guiProcessEvents(std::vector<std::string> types, std::vector<std::string> names, std::vector<std::string> funcs){
	for(int i = 0; i < types.size(); i++){
		guiManager->processEvent(types[i], names[i], funcs[i]);
	}
}
void GameManager::changeLevel(std::string newLevel){
	logComment("ChangeLevel Called");
	levelLoader->unLoadCurrLevel();
	std::cout << "Unloaded levelloader scene" << std::endl;
	// unload from other places
	audioManager->unloadLevel();
	std::cout << "Unloaded audioManager scene" << std::endl;
	guiManager->unloadLevel();
	std::cout << "Unloaded guiManager scene" << std::endl;
	//renderManager->unloadLevel("current");
	//std::cout << "Unloaded renderManager scene" << std::endl;
	levelLoader->LoadLevel(newLevel);
	std::cout << "Loaded levelloader scene" << std::endl;
	render_manager->startRendering();
	std::cout << "Resumed rendering" << std::endl;
}
void GameManager::changeMainSong(std::string song){
	levelLoader->changeMainSong(song);
}
void GameManager::unloadMainStream(){
	audioManager->unloadMainStream();
}
void GameManager::playSample(std::string sample){
	audioManager->playSample(levelLoader->getResource(sample));
}
LevelLoader* GameManager::getLevelLoader(){
	return levelLoader;
}
void GameManager::callScript(std::string scriptName, std::string functionName){
	scriptManager->executeScript(scriptName, functionName);
}
