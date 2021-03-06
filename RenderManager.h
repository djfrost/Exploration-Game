#if !defined RENDER_MANAGER
#define RENDER_MANAGER
#include <string>
#include "Ogre.h"
#include "CSC2110/ListArray.h"
#include <iostream>
#include <stdint.h>
class AnimationRenderListener;
class GameManager;
class ResourceParser;
class RenderListener;
class InputFunctionHandler;
class PhysicsManager;
struct SceneNodeMotion{
	Ogre::SceneNode* sceneNodeMotion;
};

struct SceneNodeManual{
	Ogre::SceneNode* sceneNodeManual;
};
class RenderManager{
	private:
		bool rootRendering;
		bool levelChange;
		std::string newLevel;
		Ogre::Root* root;
		Ogre::RenderWindow* window;
		Ogre::SceneManager* scene_manager;
		AnimationRenderListener* render_listener;
		Ogre::Camera* camera;
		Ogre::Viewport* viewport;
		GameManager* game_manager;
		void init();
		size_t window_handle;
		Ogre::Real time_since_last_frame;
		Ogre::ShadowTechnique shadow_technique;
		//Curr and next scene
		Ogre::SceneNode* currSceneNode;
		Ogre::SceneNode* nextSceneNode;
		bool groupExists( std::string group );
		std::vector<Ogre::AnimationState* > anims;
		ListArray<RenderListener>* render_listeners;
		InputFunctionHandler* ifh;
		void destroyAllAttachedMovableObjects( Ogre::SceneNode* node );
		PhysicsManager* physicsManager;
	public:
		RenderManager(GameManager* game_manager);
		virtual ~RenderManager();

		size_t getRenderWindowHandle();
		int getRenderWindowWidth();
		int getRenderWindowHeight();

		Ogre::RenderWindow* getRenderWindow();
		Ogre::SceneManager* getSceneManager();
		void buildSimpleScene();
		void addPath(std::string path, std::string resourceGroup);
		void addMesh(std::string mesh, std::string resourceGroup, std::string mesh_file);
		void unloadScene(std::string currScene);
		void loadScene(std::string sceneName, std::string lastScene, std::vector<std::string> meshNames, std::vector<std::string> meshFiles, std::vector< std::vector<float> > transforms, std::vector < std::vector<float> > rotates, std::vector<float> angle, std::vector < std::vector<float> > scales, std::vector<std::string> animNames);
		void loadCameras(std::vector< std::vector< float > > positions, std::vector< std::vector < float > > lookAts, std::vector<float> nearclips, std::vector<float> farclips, std::vector<float> rotation, std::vector<float> angle, std::vector<std::string> parents);
		void loadLights(std::vector<std::string> names, std::vector<float> types, std::vector< std::vector < float > > colors, std::vector< std::vector < float > > directions);
		void initialiseNewScene();
		void loadSkyBox(std::string skyBoxMat);
		void processAnims(std::vector<std::string> objects, std::vector<std::string> types, std::vector< std::vector < float > > values, std::vector< std::vector < float > >  axis, std::vector< std::vector < float > > timeSteps, std::vector< std::vector < float > > start, std::vector < std::vector<float> > begin );
		void processAnimations(float timestep);
		void processKeyboardInput(std::string keyName, bool released);
		void checkForInput(float time_step);
		void leftJoystickAxisMoved(float north_south, float east_west);
		void rightJoystickAxisMoved(float north_south, float east_west);
		void startRendering();
		void stopRendering();
		void logComment(std::string comment);
		void destroySceneNode( Ogre::SceneNode* node);
		void updateAudio(float time_step);
		void mousePressed(int x_click, int y_click, int mouseButton);
		void mouseReleased(int x_click, int y_click, int mouseButton);
		void mouseMoved(int x_click, int y_click, int x_rel, int y_rel);
		void playAudio(const char fileName[]);
		void changeLevel(std::string level);
		void changeMainSong(std::string song);
		void playSample(std::string sample);
		SceneNodeManual* createManualObject();
		void drawLine(float* from, float* to, float* color, SceneNodeManual* snm);
		void setOrientation(SceneNodeMotion* sceneNodeMotion, float x, float y, float z, float w);
		void setPosition(SceneNodeMotion* sceneNodeMotion, float x, float y, float z);
		void createCollisionShape(std::string& childName, std::string& shapeName, float mass, float* trans, float* rot,float* params);
		void applyPulse(float northSouth, float eastWest, std::string& nodeName);
		void setGravity(float* gravity);
		void stepPhysicsSimulation(float elapsedTime);
		void setDebugDrawer(bool debugMode);
		void createRigidBodies();
		SceneNodeMotion* createSceneNodeMotion(std::string& sceneNodeId);
		void destroySceneNodeMotion(SceneNodeMotion* snm);
		void clearManualObject(SceneNodeManual* snm);
		float* getPosition(SceneNodeMotion* sceneNodeMotion);
		float* getOrientation(SceneNodeMotion* sceneNodeMotion);
		void applyImpulse(std::string& rigidBodyName, float ns, float ew,float other);
		void applyTorqueImpulse(std::string rigidBodyName, float pitch, float yaw, float roll);
};

#endif
