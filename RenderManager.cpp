#include "RenderManager.h"
#include "GameManager.h"

#include <iostream>
using namespace std;
using namespace Ogre;
void RenderManager::init(){
	root = NULL;
	window = NULL;
	scene_manager = NULL;

	root = OGRE_NEW Ogre::Root("","");  //resource/config files go here
	root->loadPlugin("RenderSystem_GL");  //prepares external dlls for later use

	Ogre::RenderSystem* render_system = root->getRenderSystemByName("OpenGL Rendering Subsystem"); //just returns a pointer to an uninialized render system
	if (!render_system){
	 //ASSERT_CRITICAL(false);
	}

	root->setRenderSystem(render_system);
	//manually set configuration options
	render_system->setConfigOption("Full Screen", "No");
	render_system->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	//initialize render system
	//automatically create the window and give it a title
	window = root->initialise(true, "Explore");  

	scene_manager = root->createSceneManager(Ogre::ST_GENERIC, "Default Scene Manager");
	window->getCustomAttribute("WINDOW", &window_handle);

	//the Ogre viewport corresponds to a clipping region into which the contents of the camera view will be rendered in the window on each frame
	//by default, the size of the viewport matches the size of the window, but the viewport can be cropped
	//the camera represents a view into an existing scene and the viewport represents a region into which an existing camera will render its contents
	camera = scene_manager->createCamera("Camera");

	//z-order, left, top, width, height
	viewport = window->addViewport(camera, 0, 0.0, 0.0, 1.0, 1.0);  //assign a camera to a viewport (can have many cameras and viewports in a single window)
	viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	float actual_width = Ogre::Real(viewport->getActualWidth());
	float actual_height = Ogre::Real(viewport->getActualHeight());
	float aspect_ratio = actual_width/actual_height;
	camera->setAspectRatio(aspect_ratio);
	//buildSimpleScene();
}

RenderManager::RenderManager(GameManager* gm){
	game_manager = gm;
	init();
}

RenderManager::~RenderManager(){
   game_manager = NULL;
   
   scene_manager->clearScene();
   scene_manager->destroyAllCameras();

   window->removeAllViewports();

   window->destroy();
   window = NULL;

   delete root;
   root = NULL;
}

size_t RenderManager::getRenderWindowHandle(){
	return window_handle;
}

int RenderManager::getRenderWindowWidth(){
	return viewport->getActualWidth();
}

int RenderManager::getRenderWindowHeight(){
	return viewport->getActualHeight();
}

void RenderManager::startRendering(){
	root->startRendering();
}

Ogre::RenderWindow* RenderManager::getRenderWindow(){
	return window;
}

Ogre::SceneManager* RenderManager::getSceneManager(){
   return scene_manager;
}

void RenderManager::buildSimpleScene(){
	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.addResourceLocation("./Assets/Models", "FileSystem", "Level_1");
	rgm.addResourceLocation("./Assets/Materials/scripts", "FileSystem", "Level_1");
	rgm.declareResource("Fox.mesh","Mesh","Level_1");
	rgm.declareResource("Torus.mesh","Mesh","Level_1");
	rgm.initialiseResourceGroup("Level_1");
	rgm.loadResourceGroup("Level_1", true, true);
	
	camera->setPosition(Ogre::Vector3(0,0,10));
	camera->lookAt(Ogre::Vector3(0,0,0));
	camera->setNearClipDistance(2);
	camera->setFarClipDistance(50);
	scene_manager->setAmbientLight(Ogre::ColourValue(.05,.05,.05));
	Ogre::Light* light = scene_manager->createLight("Light");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	
	light->setDiffuseColour(1.0,1.0,1.0);
	light->setDirection(Ogre::Vector3(0.0,0.0,-1.0));
	
	Ogre::SceneNode* torus_node = scene_manager->createSceneNode("Torus_Node");
	Ogre::SceneNode* fox_node = scene_manager->createSceneNode("Fox_Node");
	
	Ogre::Entity* torus_entity = scene_manager->createEntity("Torus_Entity","Torus.mesh");
	Ogre::Entity* fox_entity = scene_manager->createEntity("Fox_Entity","Fox.mesh");
	
	torus_node->attachObject(torus_entity);
	fox_node->attachObject(fox_entity);
	Ogre::SceneNode* scene_root_node = scene_manager->getRootSceneNode();
	scene_root_node->addChild(torus_node);
	scene_root_node->addChild(fox_node);
	
	Vector3 vr(1,0,0);
	Quaternion q(Degree(60),vr);
	torus_node->rotate(q);
	torus_node->translate(0.0,1.0,0.0);
	torus_node->scale(1,2,1);
}

void RenderManager::addPath(std::string path, std::string resourceGroup){
	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.addResourceLocation(path, "FileSystem", resourceGroup);
}
void RenderManager::addMesh(std::string mesh, std::string resourceGroup){
	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.declareResource(mesh, "Mesh", resourceGroup);
}
void RenderManager::unloadScene(std::string currScene){
	if(groupExists(currScene)){
		Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
		rgm.unloadResourceGroup(currScene.c_str());
		currSceneNode->removeAndDestroyAllChildren();
		delete currSceneNode;
	}
}
void RenderManager::loadScene(std::string sceneName, std::string lastScene){
	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
	unloadScene(lastScene);
	rgm.initialiseResourceGroup(sceneName);
	rgm.loadResourceGroup(sceneName, true, true);
}
void RenderManager::initialiseNewScene(){
	nextSceneNode = scene_manager->createSceneNode("Base_Level_Node");
}

bool RenderManager::groupExists( std::string group )
{
   Ogre::StringVector groupNameList = Ogre::ResourceGroupManager::getSingleton().getResourceGroups(); 
   Ogre::StringVector::iterator resGroupIter = groupNameList.begin(); 

   for(;resGroupIter < groupNameList.end();resGroupIter++)
   { 
      Ogre::String resGroupName = (*resGroupIter); 
      if( resGroupName == group )
      {
             return true;
       } 
   }
   return false;
}