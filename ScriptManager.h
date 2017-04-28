#if !defined SCRIPT_MANAGER
#define SCRIPT_MANAGER

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}
//#include "luawrapper.hpp"
//#include "luawrapperutil.hpp"

#include <string>
#include <vector>

class RenderManager;

class ScriptManager{
	private:
		lua_State* L;
		RenderManager* rm;
		std::vector<const char*>* inputs;
		std::vector<const char*>* outputs;
	public:
		ScriptManager(RenderManager* render_manager);
		virtual ~ScriptManager();

		void input(std::string& input);
		std::string output(int index);
		void reset();
		void executeScript(std::string& file_name, std::string& script_function_name, int num_outputs);
};
#endif
