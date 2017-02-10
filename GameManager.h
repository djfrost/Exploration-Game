#if !defined GAME_MANAGER
#define GAME_MANAGER

#include <string>
#include <vector>
class RenderManager;
class LevelLoader;
//supplies communication between managers
class GameManager{
	private:
		RenderManager* render_manager;
		LevelLoader* levelLoader;
		GameManager();
		void init();

	public:
		virtual ~GameManager();
		static GameManager* getGameManager();

		int getRenderWindowWidth();
		int getRenderWindowHeight();
		size_t getRenderWindowHandle();
		std::string getLoadedLevelName();
		void addPath(std::string path, std::string resourceGroup);
		void addMesh(std::string mesh, std::vector<float> transform, std::vector<float> rotate, std::vector<float> scale, std::string resourceGroup);
		void loadScene(std::string scene, std::string lastScene);
		void initialiseNewScene();
};

#endif
