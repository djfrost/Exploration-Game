#if !defined RENDER_MANAGER
#define RENDER_MANAGER
#include <string>
#include "Ogre.h"

class GameManager;
class ResourceParser;
class RenderManager{
	private:
		Ogre::Root* root;
		Ogre::RenderWindow* window;
		Ogre::SceneManager* scene_manager;

		Ogre::Camera* camera;
		Ogre::Viewport* viewport;

		GameManager* game_manager;
		void init();
		size_t window_handle;
		Ogre::Real time_since_last_frame;
		//Curr and next scene
		Ogre::SceneNode* currSceneNode;
		Ogre::SceneNode* nextSceneNode;
		bool groupExists( std::string group );
	public:
		RenderManager(GameManager* game_manager);
		virtual ~RenderManager();

		size_t getRenderWindowHandle();
		int getRenderWindowWidth();
		int getRenderWindowHeight();

		Ogre::RenderWindow* getRenderWindow();
		Ogre::SceneManager* getSceneManager();

		void startRendering();
		void stopRendering();

		void buildSimpleScene();
		void addPath(std::string path, std::string resourceGroup);
		void addMesh(std::string mesh, std::string resourceGroup);
		void unloadScene(std::string currScene);
		void loadScene(std::string sceneName, std::string lastScene);
		void initialiseNewScene();
};

#endif
