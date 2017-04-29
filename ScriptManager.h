#if !defined SCRIPT_MANAGER
#define SCRIPT_MANAGER

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}
#include <LuaBridge.h>

#include <string>
#include <vector>

class GameManager;

class ScriptManager{
	private:
		lua_State* L;
		GameManager* gm;
		std::vector<const char*>* inputs;
		std::vector<const char*>* outputs;
	public:
		ScriptManager(GameManager* game_manager);
		virtual ~ScriptManager();

		void input(std::string& input);
		std::string output(int index);
		void reset();
		void executeScript(std::string& file_name);
		void executeScript(std::string& file_name, std::string& script_function_name);
		void executeScript(std::string& file_name, std::string& script_function_name, int num_outputs);
};
#endif
