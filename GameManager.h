#if !defined GAME_MANAGER
#define GAME_MANAGER

#include <string>
#include <vector>
class RenderManager;
class LevelLoader;
class LogManager;
class InputManager;
//supplies communication between managers
class GameManager{
	private:
		RenderManager* render_manager;
		LevelLoader* levelLoader;
		LogManager* logManager;
		InputManager* inputManager;
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
		void addMesh(std::string mesh, std::vector<float> transform, std::vector<float> rotate, std::vector<float> scale, std::string resourceGroup, std::string mesh_file);
		void loadScene(std::string scene, std::string lastScene, std::vector<std::string> meshNames, std::vector<std::string> meshFiles, std::vector < std::vector<float> > transforms, std::vector < std::vector<float> > rotates, std::vector<float> angle, std::vector < std::vector<float> > scales, std::vector<std::string> animNames);
		void loadCameras(std::vector< std::vector< float > > positions, std::vector< std::vector < float > > lookAts, std::vector<float> nearclips, std::vector<float> farclips);
		void loadLights(std::vector<std::string> names, std::vector<float> types, std::vector< std::vector < float > > colors, std::vector< std::vector < float > > directions);
		void loadSkyBox(std::string skyBoxMat);
		void processAnims(std::vector<std::string> objects, std::vector<std::string> types, std::vector< std::vector < float > > values, std::vector< std::vector < float > >  axis, std::vector< std::vector < float > > timeSteps, std::vector< std::vector < float > > start, std::vector < std::vector<float> > begin);
		void initialiseNewScene();
		void logComment(std::string comment);
		void checkForInput(float time_step);
		void keyPressed(std::string keyPressed);
		bool keyReleased(std::string keyUp);
		void logProblem(std::string prob, std::string file, int line);
		void leftJoystickAxisMoved(float north_south, float east_west);
		void rightJoystickAxisMoved(float north_south, float east_west);
};

#endif
