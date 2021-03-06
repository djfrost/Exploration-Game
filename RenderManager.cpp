#include "RenderManager.h"
#include "GameManager.h"
#include "AnimationRenderListener.h"
#undef OGRE_MAX_NUM_BONES
#define OGRE_MAX_NUM_BONES 20000

#include "CSC2110/ListArrayIterator.h"
#include "InputRenderListener.h"
#include "InputFunctionHandler.h"
#include "PhysicsManager.h"
using namespace std;
using namespace Ogre;



void RenderManager::updateAudio(float time_step){
	game_manager->updateAudio(time_step);
}
void RenderManager::startRendering(){
	std::cout << "Getting iterator" << std::endl;
	ListArrayIterator<RenderListener>* render_listeners_iter = render_listeners->iterator();
	while(render_listeners_iter->hasNext()){
		std::cout << "Processing next listener" << std::endl;
		RenderListener* render_listener = render_listeners_iter->next();
		render_listener->startRendering();
	}
	delete render_listeners_iter;
	std::cout << "Starting root render" << std::endl;
	if(!rootRendering){
		rootRendering = true;
		root->startRendering();
	}
}

void RenderManager::stopRendering(){
	ListArrayIterator<RenderListener>* render_listeners_iter = render_listeners->iterator();
	while(render_listeners_iter->hasNext()){
		RenderListener* render_listener = render_listeners_iter->next();
		render_listener->stopRendering();
	}
	delete render_listeners_iter;
	//root->stopRendering();
}

void RenderManager::leftJoystickAxisMoved(float north_south, float east_west){
	ifh->leftStickPressed(north_south, east_west);
}

void RenderManager::rightJoystickAxisMoved(float north_south, float east_west){
	ifh->rightStickPressed(north_south, east_west);
}

void RenderManager::init(){
	shadow_technique = SHADOWTYPE_TEXTURE_ADDITIVE ;
	rootRendering = false;
	root = NULL;
	window = NULL;
	scene_manager = NULL;
	std::cout << "Set rootRendering to false" << std::endl;
	std::cout << "complete" << std::endl;
	root = OGRE_NEW Ogre::Root("","");  //resource/config files go here
	std::cout << "Ogre root initialised" << std::endl;
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
	scene_manager->setShadowTechnique(shadow_technique);
	//scene_manager->setSceneBlending( Ogre::SceneBlendType::SBT_TRANSPARENT_COLOUR );
	//z-order, left, top, width, height
	viewport = window->addViewport(camera, 0, 0.0, 0.0, 1.0, 1.0);  //assign a camera to a viewport (can have many cameras and viewports in a single window)
	viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	float actual_width = Ogre::Real(viewport->getActualWidth());
	float actual_height = Ogre::Real(viewport->getActualHeight());
	float aspect_ratio = 16/9;
	camera->setAspectRatio(aspect_ratio);
	//buildSimpleScene();
	ifh = new InputFunctionHandler(this);
	render_listener = new AnimationRenderListener(this);
	root->addFrameListener(render_listener);
	render_listeners = new ListArray<RenderListener>();
	RenderListener* inListener = new InputRenderListener(this);
	root->addFrameListener(inListener);
	render_listeners->add(inListener);
}

RenderManager::RenderManager(GameManager* gm){
	levelChange = false;
	game_manager = gm;
	init();
	std::cout << "Constructing physics manager " << std::endl;
	physicsManager = new PhysicsManager(this);
	std::cout << "Constructed physics manager " << std::endl;
}

RenderManager::~RenderManager(){
   render_listener->stopRendering();
   std::cout << "Rendering Stopped" << std::endl;
   delete render_listener;
   std::cout << "render_listener destroyed" << std::endl;
   render_listener = NULL;
   game_manager = NULL;
   std::cout << "GameManager reference gone" << std::endl;
   for(int i = 0; i < anims.size(); i++){
	   anims[i] = NULL;
   }
   delete physicsManager;
   std::cout << "Animation states deleted" << std::endl;
   render_listeners->removeAll();
   delete render_listeners;
   std::cout << "Render listeners deleted" << std::endl;
   delete ifh;
   std::cout << "input handler functions deleted" << std::endl;
   scene_manager->clearScene();
   std::cout << "Scene cleared" << std::endl;
   scene_manager->destroyAllCameras();
   std::cout << "Cameras destroyed" << std::endl;
   window->removeAllViewports();
   std::cout << "Viewports gone" << std::endl;
   window->destroy();
   window = NULL;
   std::cout << "Window destroyed" << std::endl;
   delete scene_manager;
   std::cout << "scene_manager deleted" << std::endl;
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
void RenderManager::addMesh(std::string mesh, std::string resourceGroup, std::string mesh_file){
	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.declareResource(mesh, "Mesh", resourceGroup);
	Ogre::SceneNode* new_node = scene_manager->createSceneNode(mesh + "_node");
	Ogre::Entity* new_entity = scene_manager->createEntity(mesh+"_entity",mesh_file);
	new_node->attachObject(new_entity);
	Ogre::SceneNode* scene_root_node = scene_manager->getRootSceneNode();
	scene_root_node->addChild(new_node);
}
//destroy old scene nodes, and unloads resource gruop if the scene exists.
void RenderManager::unloadScene(std::string currScene){
	game_manager->logComment("Level is being unloaded");
	std::cout << currScene << "Is being unloaded" << std::endl;
	if(groupExists(currScene)){
		scene_manager->destroyAllCameras();
		Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
		rgm.unloadResourceGroup(currScene.c_str());
		Ogre::SceneNode::ChildNodeIterator itChild = scene_manager->getRootSceneNode()->getChildIterator();

	    while ( itChild.hasMoreElements() )
	    {
	       Ogre::SceneNode* pChildNode = static_cast<SceneNode*>(itChild.getNext());
	       destroyAllAttachedMovableObjects( pChildNode );
		}
		for(int i = 0; i < anims.size(); i++){
		   anims[i] = NULL;
		}
		anims.clear();
		rgm.destroyResourceGroup(currScene.c_str());
		scene_manager->destroyAllEntities();
		scene_manager->destroyAllAnimations();
		scene_manager->destroyAllAnimationStates();
		scene_manager->destroyAllInstancedGeometry();
		scene_manager->destroyAllMovableObjects();
		scene_manager->destroyAllStaticGeometry();
		scene_manager->destroyAllLights();
		scene_manager->getRootSceneNode()->removeAndDestroyAllChildren();
		scene_manager->destroyAllCameras();
		camera = scene_manager->createCamera("Camera");
		scene_manager->setShadowTechnique(shadow_technique);
		//scene_manager->setSceneBlending( Ogre::SceneBlendType::SBT_TRANSPARENT_COLOUR );
		//z-order, left, top, width, height
		window->removeAllViewports();
		viewport = window->addViewport(camera, 0, 0.0, 0.0, 1.0, 1.0);  //assign a camera to a viewport (can have many cameras and viewports in a single window)
		viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
		float actual_width = Ogre::Real(viewport->getActualWidth());
		float actual_height = Ogre::Real(viewport->getActualHeight());
		float aspect_ratio = 16/9;
		camera->setAspectRatio(aspect_ratio);
	}
}
//Unload the last scene, then initialise the current resource group and load it.
void RenderManager::loadScene(std::string sceneName, std::string lastScene, std::vector<std::string> meshNames, std::vector<std::string> meshFiles, std::vector< std::vector<float> >transforms, std::vector < std::vector<float> > rotates, std::vector<float> angle, std::vector < std::vector<float> > scales, std::vector<std::string> animNames ){
	//Temp code
	std::cout << "Loading scene " << std::endl;
	scene_manager->setAmbientLight(Ogre::ColourValue(.30,.30,.30));
	//End of temp code
	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
	unloadScene(lastScene);
	for(int i = 0; i < meshFiles.size(); i++){
		rgm.declareResource(meshFiles[i], "Mesh", sceneName);
	}
	rgm.initialiseResourceGroup(sceneName);
	rgm.loadResourceGroup(sceneName, true, true);
	Ogre::SceneNode* scene_root_node = scene_manager->getRootSceneNode();
	for(int i = 0; i < meshFiles.size(); i++){
		Ogre::SceneNode* parent_node = scene_manager->createSceneNode(meshNames[i] + "_transform");
		Ogre::SceneNode* fox_node = parent_node->createChildSceneNode(meshNames[i] + "_node");
		Ogre::Entity* fox_entity = scene_manager->createEntity(meshNames[i] + "_Entity",meshFiles[i]);
		fox_node->attachObject(fox_entity);
		parent_node->translate(transforms[i][0],transforms[i][1],transforms[i][2]);
		Vector3 vr(rotates[i][0],rotates[i][1],rotates[i][2]);
		Quaternion q(Degree(angle[i]),vr);
		fox_node->rotate(q);
		fox_node->scale(scales[i][0],scales[i][1],scales[i][2]);
		scene_root_node->addChild(parent_node);
		if(animNames[i] != "none"){
			Ogre::AnimationState* animState = fox_entity->getAnimationState(animNames[i]);
			animState->setLoop(true);
			animState->setEnabled(true);
			anims.push_back(animState);
		}

	}
}
void RenderManager::loadCameras(std::vector< std::vector< float > > positions, std::vector< std::vector < float > > lookAts, std::vector<float> nearclips, std::vector<float> farclips, std::vector<float> rotation, std::vector<float> angle, std::vector<std::string> parents){
	camera->setPosition(Ogre::Vector3(positions[0][0], positions[0][1], positions[0][2]));
	camera->lookAt(Ogre::Vector3(lookAts[0][0], lookAts[0][1], lookAts[0][2]));
	camera->setNearClipDistance(nearclips[0]);
	camera->setFarClipDistance(0);
	std::cout << "Parents[0]" << parents[0] << std::endl;
	if(parents[0] != "none"){
			Ogre::SceneNode* parentNode = scene_manager->getSceneNode(parents[0] + "_node");
			Ogre::SceneNode* rotNode = parentNode->createChildSceneNode("camRot_node");
			Ogre::SceneNode* cameraNode = rotNode->createChildSceneNode("CameraNode");
			cameraNode->attachObject(camera);
			Vector3 vr(rotation[0],rotation[1],rotation[2]);
			Quaternion q(Degree(angle[0]),vr);
			rotNode->rotate(q);
		}
}
void RenderManager::loadLights(std::vector<std::string> names, std::vector<float> types, std::vector< std::vector < float > > colors, std::vector< std::vector < float > > directions){
	for(int i = 0; i < types.size(); i++){
		Ogre::Light* light = scene_manager->createLight(names[i]);
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDiffuseColour(colors[i][0],colors[i][1],colors[i][2]);
		light->setDirection(Ogre::Vector3(directions[i][0],directions[i][1],directions[i][2]));
	}
}
//Get a new scene node
void RenderManager::initialiseNewScene(){
	nextSceneNode = scene_manager->createSceneNode("Base_Level_Node");
}

/*
	Checks if a given resource group exists. If so returns true else false
*/
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
void RenderManager::loadSkyBox(std::string skyBoxMat){
	scene_manager->setSkyBox(true, skyBoxMat);
}
void RenderManager::processAnims(std::vector<std::string> objects, std::vector<std::string> types, std::vector< std::vector < float > > values, std::vector< std::vector < float > >  axis, std::vector< std::vector < float > > timeSteps, std::vector< std::vector < float > > start, std::vector < std::vector<float> > begin){
	for(int i =0; i < objects.size(); i++){
		Ogre::Animation* newAnim = scene_manager->createAnimation(objects[i] + "Anim", timeSteps[i][0]);
		newAnim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
		Ogre::SceneNode* scene_root_node = scene_manager->getRootSceneNode();
		Ogre::Node* animNode = scene_root_node->getChild(objects[i] + "_node");
		Ogre::NodeAnimationTrack* animTrack = newAnim->createNodeTrack(1,animNode);
		if(types[i] == "rot"){
			Ogre::TransformKeyFrame* animFrame = animTrack->createNodeKeyFrame(start[i][0]);
			Ogre::Quaternion rotQ(Degree(begin[i][0]), Ogre::Vector3(axis[i][0],axis[i][1], axis[i][2]));
			animFrame->setRotation(rotQ);
			Ogre::TransformKeyFrame* animFrameEnd = animTrack->createNodeKeyFrame(timeSteps[i][0]);
			Ogre::Quaternion rotQ2(Degree(values[i][0]), Ogre::Vector3(axis[i][0],axis[i][1], axis[i][2]));
			animFrameEnd->setRotation(rotQ2);
		}
		Ogre::AnimationState* anim = scene_manager->createAnimationState(objects[i] + "Anim");
		anim->setEnabled(true);
		anim->setLoop(true);
		anims.push_back(anim);
	}
}
void RenderManager::processAnimations(float timestep){
	for(int i = 0; i < anims.size(); i++){
		anims[i]->addTime(timestep);
	}
	ifh->keyDownFunctions();
	if(levelChange){
		levelChange = false;
		game_manager->changeLevel(newLevel);
		std::cout <<"Returned from changeLevel" << std::endl;
	}
}
void RenderManager::processKeyboardInput(std::string keyName, bool released){
	ifh->processInput(keyName, released);
}
void RenderManager::checkForInput(float time_step){
	game_manager->checkForInput(time_step);
}
void RenderManager::logComment(std::string comment){
	game_manager->logComment(comment);
}
void RenderManager::destroyAllAttachedMovableObjects( Ogre::SceneNode* node )
{
   if(!node) return;

   // Destroy all the attached objects
   Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

   while ( itObject.hasMoreElements() )
      node->getCreator()->destroyMovableObject(itObject.getNext());

   // Recurse to child SceneNodes
   Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      Ogre::SceneNode* pChildNode = static_cast<SceneNode*>(itChild.getNext());
      destroyAllAttachedMovableObjects( pChildNode );
   }
}

void RenderManager::destroySceneNode( Ogre::SceneNode* node )
{
   if(!node) return;
   destroyAllAttachedMovableObjects(node);
   node->removeAndDestroyAllChildren();
   node->getCreator()->destroySceneNode(node);
}
void RenderManager::mousePressed(int x_click, int y_click, int mouseButton){}
void RenderManager::mouseReleased(int x_click, int y_click, int mouseButton){}
void RenderManager::mouseMoved(int x_click, int y_click, int x_rel, int y_rel){}
void RenderManager::playAudio(const char fileName[]){}
void RenderManager::changeLevel(std::string level){
	levelChange = true;
	newLevel = level;
}
void RenderManager::changeMainSong(std::string song){
	game_manager->changeMainSong(song);
}
void RenderManager::playSample(std::string sample){
	game_manager->playSample(sample);
}
void RenderManager::setOrientation(SceneNodeMotion* sceneNodeMotion, float x, float y, float z, float w){
	Ogre::SceneNode* sceneNode = sceneNodeMotion->sceneNodeMotion;
	sceneNode->setOrientation(x,y,z,w);
}
void RenderManager::setPosition(SceneNodeMotion* sceneNodeMotion, float x, float y, float z){
	Ogre::SceneNode* sceneNode = sceneNodeMotion->sceneNodeMotion;
	sceneNode->setPosition(x,y,z);
}
float* RenderManager::getOrientation(SceneNodeMotion* sceneNodeMotion){
	Ogre::SceneNode* sceneNode = sceneNodeMotion->sceneNodeMotion;
	Ogre::Quaternion q = sceneNode->getOrientation();
	Ogre::Real w = q.w;
	Ogre::Real x = q.x;
	Ogre::Real y = q.y;
	Ogre::Real z = q.z;
	float* rot = new float[4];
	rot[3] = w;
	rot[0] = x;
	rot[1] = y;
	rot[2] = z;
	return rot;
}
float* RenderManager::getPosition(SceneNodeMotion* sceneNodeMotion){
	Ogre::SceneNode* sceneNode = sceneNodeMotion->sceneNodeMotion;
	Ogre::Vector3 position = sceneNode->getPosition();
	float* pos = new float[3];
	pos[0] = position.x;
	pos[1] = position.y;
	pos[2] = position.z;
	return pos;
}
void RenderManager::clearManualObject(SceneNodeManual* snm){
	Ogre::SceneNode* sceneNodeManual = snm->sceneNodeManual;
	Ogre::ManualObject* manualObject = (Ogre::ManualObject*) sceneNodeManual->getAttachedObject(0);
	manualObject->clear();
}
SceneNodeManual* RenderManager::createManualObject(){
	SceneNodeManual* sceneNodeManual = new SceneNodeManual;
	Ogre::ManualObject* manualObject = scene_manager->createManualObject("Manual_Object");
	manualObject->setDynamic(true);
	static const char* mat_name = "OgreBulletCollisionsDebugDefault";
	Ogre::MaterialPtr manualObjectMaterial = Ogre::MaterialManager::getSingleton().getDefaultSettings();
	manualObjectMaterial->setReceiveShadows(false);
	manualObjectMaterial->getTechnique(0)->setLightingEnabled(false);
	Ogre::SceneNode* manualObjectNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	manualObjectNode->attachObject(manualObject);
	sceneNodeManual->sceneNodeManual = manualObjectNode;
	return sceneNodeManual;
}
void RenderManager::drawLine(float* from, float* to, float* color, SceneNodeManual* snm){
	Ogre::SceneNode* sceneNodeManual = snm->sceneNodeManual;
	Ogre::ManualObject* manualObject = (Ogre::ManualObject*)sceneNodeManual->getAttachedObject(0);
	manualObject->begin("OgreBulletCollisionsDebugDefault", Ogre::RenderOperation:: OT_LINE_LIST);
	manualObject->position(Ogre::Vector3(from[0], from[1], from[2]));
	manualObject->colour(Ogre::ColourValue(color[0], color[1], color[2]));
	manualObject->position(Ogre::Vector3(to[0], to[1], to[2]));
	manualObject->colour(Ogre::ColourValue(color[0],color[1], color[2]));
	manualObject->end();
}
void RenderManager::destroySceneNodeMotion(SceneNodeMotion* snm){
	free(snm);
}
SceneNodeMotion* RenderManager::createSceneNodeMotion(std::string& sceneNodeId){
	SceneNodeMotion* sceneNodeMotion = (SceneNodeMotion*) malloc(sizeof(SceneNodeMotion));
	sceneNodeMotion->sceneNodeMotion = scene_manager->getSceneNode(sceneNodeId);
	return sceneNodeMotion;
}
void RenderManager::stepPhysicsSimulation(float elapsedTime){
	physicsManager->stepPhysicsSimulation(elapsedTime);
}
void RenderManager::createRigidBodies(){
	physicsManager->createRigidBodies();
}
void RenderManager::createCollisionShape(std::string& childName, std::string& shapeName, float mass, float* trans, float* rot,float* params){
	physicsManager->createCollisionShape(childName, shapeName, mass, trans, rot, params);
}
void RenderManager::applyPulse(float northSouth,float eastWest, std::string& nodeName){
	physicsManager->applyImpulse(nodeName, northSouth, eastWest, 0.0);
}
void RenderManager::setGravity(float* gravity){
	physicsManager->setGravity(gravity);
}
void RenderManager::setDebugDrawer(bool debugMode){
	physicsManager->setDebugDrawer(debugMode);
}
void RenderManager::applyImpulse(std::string& rigidBodyName, float ns, float ew,float other){
	physicsManager->applyImpulse(rigidBodyName, ns, ew, other);
}
void RenderManager::applyTorqueImpulse(std::string rigidBodyName, float pitch, float yaw, float roll){
	physicsManager->applyImpulse(rigidBodyName, pitch, yaw, roll);
}
