#if !defined INPUT_FUNCTION_HANDLER
#define INPUT_FUNCTION_HANDLER
#include <string>
#include <vector>
class RenderManager;
class InputFunctionHandler{
	private:
		std::vector<std::string> inputs;
		RenderManager* rm;
		std::vector<void (*)(RenderManager* rm)> functions;
		std::vector<bool> bools;
	public:
		InputFunctionHandler(RenderManager* render_manager);
		virtual ~InputFunctionHandler();
		void processInput(std::string game_key, bool released);
		void keyDownFunctions();
};

#endif