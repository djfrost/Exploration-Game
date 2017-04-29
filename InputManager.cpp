#include "GameHeader.h"
#include "InputManager.h"
#include "GameManager.h"

#include <sstream>
#include <iostream>
#include <math.h>
using namespace std;

//called on every frame
void InputManager::checkForInput(){
	if(keyboard_ois){
		keyboard_ois->capture();
	}
	if(mouse_ois){
		mouse_ois->capture();
	}
	if(joystick_ois){
		joystick_ois->capture();
		OIS::JoyStickState joystick_state = joystick_ois->getJoyStickState();
		OIS::JoyStickEvent e(joystick_ois, joystick_state);
		axisMoved(e,0);
	}
}

bool InputManager::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id){
	uint32 x_click = e.state.X.abs;
	uint32 y_click = e.state.Y.abs;
	game_manager->mousePressed(x_click, y_click, mouseMap(e));
	//game_manager->mousePressed(x_click, y_click, 2);
	return true;
}
bool InputManager::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id){
	uint32 x_click = e.state.X.abs;
	uint32 y_click = e.state.Y.abs;
	game_manager->mouseReleased(x_click, y_click, mouseMap(e));
	//game_manager->mouseReleased(x_click, y_click, " ");
	return true;

}
bool InputManager::mouseMoved(const OIS::MouseEvent& e){
	e.state.width = window_width;
	e.state.height = window_height;
	uint32 x_click = e.state.X.abs;
	uint32 y_click = e.state.Y.abs;
	float x_rel = e.state.X.rel;
	float y_rel = e.state.Y.rel;
	game_manager->mouseMoved(x_click, y_click, x_rel, y_rel);
	return true;
}
bool InputManager::keyPressed(const OIS::KeyEvent& e){
	//cout << "KP" << endl;
	std::string key = keyMap(e);
	if(scriptKeyMap.find(key) == scriptKeyMap.end()){
			game_manager->keyPressed(key);
	}
	else{
		game_manager->keyPressed(key, scriptKeyMap[key]);
	}
	return true;
}

//callback
bool InputManager::keyReleased(const OIS::KeyEvent& e){
	//cout << "KR" << endl;
	game_manager->keyReleased(keyMap(e));
	return true;
}

void InputManager::init(){
	try{
		OIS::ParamList p1;
		std::ostringstream windowHndStr;
		size_t window_handle = game_manager->getRenderWindowHandle();

		size_t handle = window_handle;
		windowHndStr << handle;
		p1.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));
		input_manager_ois = OIS::InputManager::createInputSystem(p1);
		if(input_manager_ois->getNumberOfDevices(OIS::OISKeyboard) > 0){
			keyboard_ois = static_cast<OIS::Keyboard*>(input_manager_ois->createInputObject(OIS::OISKeyboard, true));
			keyboard_ois->setEventCallback(this); //forwards the information to all registered listeners
		}

		if(input_manager_ois->getNumberOfDevices(OIS::OISMouse) > 0){
			mouse_ois = static_cast<OIS::Mouse*>(input_manager_ois->createInputObject(OIS::OISMouse, true));
			mouse_ois->setEventCallback(this);
		}

		if(input_manager_ois->getNumberOfDevices(OIS::OISJoyStick) > 0){
			joystick_ois = static_cast<OIS::JoyStick*>(input_manager_ois->createInputObject(OIS::OISJoyStick, true));
			joystick_ois->setEventCallback(this);
		}
	}
	catch(std::exception& e){
		ASSERT_CRITICAL(false, e.what());
	}
	catch(...){
		ASSERT_CRITICAL(false, "Input Manager Initialization Error");
	}
}

InputManager::InputManager(GameManager* gm){
	game_manager = gm;
	input_manager_ois = NULL;
	keyboard_ois = NULL;
	mouse_ois = NULL;
	joystick_ois = NULL;
	init();

	window_width = game_manager->getRenderWindowWidth();
	window_height = game_manager->getRenderWindowHeight();
}

void InputManager::free(){
	if(input_manager_ois){
		input_manager_ois->destroyInputSystem(input_manager_ois);
	}
}

InputManager::~InputManager(){
	free();
	input_manager_ois = NULL;
	keyboard_ois = NULL;
	mouse_ois = NULL;
	joystick_ois = NULL;
}

std::string InputManager::keyMap(const OIS::KeyEvent& e){
	std::string game_key = "INVALID_KEY";
	OIS::KeyCode key_code = e.key;

	if(key_code == OIS::KC_ESCAPE){
		game_key = "ESCAPE";
	}
	else if(key_code == OIS::KC_0){
		game_key = "0";
	}
	else if(key_code == OIS::KC_1){
		game_key = "1";
	}
	else if(key_code == OIS::KC_2){
		game_key = "2";
	}
	else if(key_code == OIS::KC_3){
		game_key = "3";
	}
	else if(key_code == OIS::KC_4){
		game_key = "4";
	}
	else if(key_code == OIS::KC_5){
		game_key = "5";
	}
	else if(key_code == OIS::KC_6){
		game_key = "6";
	}
	else if(key_code == OIS::KC_7){
		game_key = "7";
	}
	else if(key_code == OIS::KC_8){
		game_key = "8";
	}
	else if(key_code == OIS::KC_9){
		game_key = "9";
	}
	else if(key_code == OIS::KC_RIGHT){
		game_key = "RIGHT";
	}
	else if(key_code == OIS::KC_LEFT){
		game_key = "LEFT";
	}
	else if(key_code == OIS::KC_UP){
		game_key = "UP";
	}
	else if(key_code == OIS::KC_DOWN){
		game_key = "DOWN";
	}
	else if(key_code == OIS::KC_A){
		game_key = "A";
	}
	else if(key_code == OIS::KC_B){
		game_key = "B";
	}
	else if(key_code == OIS::KC_C){
		game_key = "C";
	}
	else if(key_code == OIS::KC_D){
		game_key = "D";
	}
	else if(key_code == OIS::KC_E){
		game_key = "E";
	}
	else if(key_code == OIS::KC_F){
		game_key = "F";
	}
	else if(key_code == OIS::KC_G){
		game_key = "G";
	}
	else if(key_code == OIS::KC_H){
		game_key = "H";
	}
	else if(key_code == OIS::KC_I){
		game_key = "I";
	}
	else if(key_code == OIS::KC_J){
		game_key = "J";
	}
	else if(key_code == OIS::KC_K){
		game_key = "K";
	}
	else if(key_code == OIS::KC_L){
		game_key = "L";
	}
	else if(key_code == OIS::KC_M){
		game_key = "M";
	}
	else if(key_code == OIS::KC_N){
		game_key = "N";
	}
	else if(key_code == OIS::KC_O){
		game_key = "O";
	}
	else if(key_code == OIS::KC_P){
		game_key = "P";
	}
	else if(key_code == OIS::KC_Q){
		game_key = "Q";
	}
	else if(key_code == OIS::KC_R){
		game_key = "R";
	}
	else if(key_code == OIS::KC_S){
		game_key = "S";
	}
	else if(key_code == OIS::KC_T){
		game_key = "T";
	}
	else if(key_code == OIS::KC_U){
		game_key = "U";
	}
	else if(key_code == OIS::KC_V){
		game_key = "V";
	}
	else if(key_code == OIS::KC_W){
		game_key = "W";
	}
	else if(key_code == OIS::KC_X){
		game_key = "X";
	}
	else if(key_code == OIS::KC_Y){
		game_key = "Y";
	}
	else if(key_code == OIS::KC_Z){
		game_key = "Z";
	}
	else if(key_code == OIS::KC_SPACE){
		game_key = "SPACE";
	}
	else if(key_code == OIS::KC_RETURN){
		game_key = "ENTER";
	}
	else if(key_code == OIS::KC_BACK){
		game_key = "BACK";
	}
	return game_key;
}

bool InputManager::buttonPressed(const OIS::JoyStickEvent& e, int button){return true;}
bool InputManager::buttonReleased(const OIS::JoyStickEvent& e, int button){return true;}
bool InputManager::sliderMoved(const OIS::JoyStickEvent& e, int index){return true;}
bool InputManager::povMoved(const OIS::JoyStickEvent& e, int index){return true;}
bool InputManager::vector3Moved(const OIS::JoyStickEvent& e, int index){return true;}
bool InputManager::axisMoved(const OIS::JoyStickEvent& e, int axis){
	int TOL = 10000;
	int amount[6] = {e.state.mAxes[0].abs, e.state.mAxes[1].abs, e.state.mAxes[2].abs, e.state.mAxes[3].abs, e.state.mAxes[4].abs, e.state.mAxes[5].abs};
	float north_south = 0;
	float east_west = 0;
	if(fabs(amount[0]) > TOL){
		north_south = amount[0]/32767.0;
	}
	if(fabs(amount[1]) > TOL){
		east_west = amount[1]/32767.0;
	}
	game_manager->leftJoystickAxisMoved(north_south, east_west);

	north_south = 0;
	east_west = 0;
	if(fabs(amount[2]) > TOL){
		north_south = amount[0]/32767.0;
	}
	if(fabs(amount[3]) > TOL){
		east_west = amount[1]/32767.0;
	}
	game_manager->rightJoystickAxisMoved(north_south, east_west);

	north_south = 0;
	east_west = 0;
	return true;
}
std::string InputManager::joystickAxisMap(int axis){

}
int InputManager::mouseMap(const OIS::MouseEvent& id){
	int mouseKey = 0;
	if(id.state.buttonDown(OIS::MB_Left)){
		mouseKey = 1;
	}
	else if(id.state.buttonDown(OIS::MB_Right)){
		mouseKey = 2;
	}
	return mouseKey;
}
void InputManager::loadKeyScripts(std::vector<std::string> keys, std::vector<std::string> scripts){
	for(int i = 0; i < keys.size(); i++){
		std::cout << keys[i] << " == " << scripts[i] << std::endl;
		scriptKeyMap[keys[i]] = scripts[i];
	}
}
