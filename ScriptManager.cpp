#include "ScriptManager.h"
#include "GameManager.h"
#include "LevelLoader.h"
#include <iostream>
void ScriptManager::reset(){
	inputs->clear();
	outputs->clear();
}

void ScriptManager::input(std::string& input_str){
	const char* parameter = input_str.c_str();
	inputs->push_back(parameter);
}

std::string ScriptManager::output(int index){
	const char* output = outputs->at(index);
	std::string outputStr(output);
	return outputStr;
}
ScriptManager::ScriptManager(GameManager* game_manager){
	gm = game_manager;
	L = luaL_newstate();
	luaL_openlibs(L);
	inputs = new std::vector<const char*>();
	outputs = new std::vector<const char*>();
	luabridge::getGlobalNamespace(L)
	 .beginClass<GameManager>("GameManager")
	  .addFunction("playAudio", &GameManager::playAudio)
	  .addFunction("changeLevel", &GameManager::changeLevel)
	  .addFunction("applyImpulse", &GameManager::applyImpulse)
	  .addFunction("applyTorqueImpulse", &GameManager::applyTorqueImpulse)
  	.endClass();

  	luabridge::push(L,gm);
	lua_setglobal(L,"gm");
}
ScriptManager::~ScriptManager(){
	reset();
	delete inputs;
	inputs = NULL;
	delete outputs;
	outputs = NULL;
	lua_close(L);
	gm = NULL;
}
void ScriptManager::executeScript(std::string& file_name, std::string& script_function_name, int num_outputs){
	int num_inputs = inputs->size();

	luaL_dofile(L, file_name.c_str());
	lua_getglobal(L, script_function_name.c_str());

	for(int i = 0; i < num_inputs; i++){
		const char* input = inputs->at(i);
		lua_pushstring(L, input);
	}

	lua_pcall(L, num_inputs, num_outputs, 0);

	for(int i = num_outputs-1; i >= 0; i--){
		const char* temp = lua_tostring(L, -1);
		outputs->push_back(temp);
		lua_pop(L,1);
	}

}
void ScriptManager::executeScript(std::string& file_name, std::string& script_function_name){
	int num_inputs = inputs->size();

	luaL_dofile(L, file_name.c_str());
	lua_getglobal(L, script_function_name.c_str());

	for(int i = 0; i < num_inputs; i++){
		const char* input = inputs->at(i);
		lua_pushstring(L, input);
	}

	lua_pcall(L, num_inputs, 0, 0);

}
void ScriptManager::executeScript(std::string& file_name){
	std::cout << "executing script" << std::endl;
	int num_inputs = inputs->size();
	luaL_dofile(L, file_name.c_str());
	for(int i = 0; i < num_inputs; i++){
		const char* input = inputs->at(i);
		lua_pushstring(L, input);
	}
}
