#if !defined SCRIPT_MANAGER
#define SCRIPT_MANAGER

extern "C" //prevents name mangling
{
	#include <lua.h>
	#include <luaxlib.h>
	#include <lualib.h>
}

//change line 488 in luawrapper.h to >= 502
#include "luawrapper.h"
#include "luawrapperutil.h"

#include <string> 
#include "CSC2110/ListArray.h"

class RenderManager;

class ScriptManager{
	private:
		lua_State* l;
		RenderManager* rm;
		
		ListArray<const char>* inputs;
		ListArray<const char>* outputs;
	public:
		ScriptManager(RenderManager* render_manager);
		virtual ~ScriptManager();
		
		void input(std::string& input);
		std::string output(int index);
		void reset();
		
		void executeScript(std::string& script_file_name, std::string& script_function_name, int num_outputs);
	
};

#endif