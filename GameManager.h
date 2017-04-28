#if !defined GAME_MANAGER
#define GAME_MANAGER
#include "ScriptManager.h"
#include <string>
#include <vector>
#include <stdint.h>
class RenderManager;
class LevelLoader;
class LogManager;
class InputManager;
class AudioManager;
struct AudioResourceInfo;
class AudioResource;
class ScriptManager;
class GUIManager;
//supplies communication between managers
class GameManager{
	private:
		RenderManager* render_manager;
		LevelLoader* levelLoader;
		LogManager* logManager;
		InputManager* inputManager;
		AudioManager* audioManager;
		GUIManager* guiManager;
		ScriptManager* scriptManager;
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
		void loadCameras(std::vector< std::vector< float > > positions, std::vector< std::vector < float > > lookAts, std::vector<float> nearclips, std::vector<float> farclips, std::vector<float> rotation, std::vector<float> angle, std::vector<std::string> parents);
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
		void loadSampleAudioResource(std::string audio_file_name, AudioResourceInfo* ar_info);
		void loadStreamAudioResource(std::string audio_file_name, AudioResourceInfo* ar_info);
		void unloadSampleAudioResource(AudioResourceInfo* ar_info);
		void unloadStreamAudioResource(AudioResourceInfo* ar_info);
		void playAudio(AudioResource* ar, int num_repeats);
		void updateAudio(float time_step);
		AudioResourceInfo* createAudioResourceInfo();
		void mousePressed(unsigned int x_click,unsigned int y_click, int mouseButton);
		void mouseReleased(unsigned int x_click,unsigned int y_click, int mouseButton);
		void mouseMoved(unsigned int x_click,unsigned int y_click,float x_rel,float y_rel);
		void guiLoadLevel(std::string level_name, std::string scheme, std::string font, std::string cursor, std::string tooltip, std::string layout);
		void guiProcessEvents(std::vector<std::string> types, std::vector<std::string> names, std::vector<std::string> funcs);
		void changeLevel(std::string newLevel);
		void changeMainSong(std::string song);
		void unloadMainStream();
		void playSample(std::string sample);
		void loadLevel(std::string name);
};

#endif
