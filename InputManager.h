#if !defined INPUT_MANAGER
#define INPUT_MANAGER

#include <OIS.h>
#include "CSC2110/ListArray.h"
#include <iostream>
#include <unordered_map>
class GameManager;
class InputManager: public OIS::KeyListener, public OIS::JoyStickListener, OIS::MouseListener{
	private:
		OIS::InputManager* input_manager_ois;

		OIS::Keyboard* keyboard_ois;
		OIS::Mouse* mouse_ois;
		OIS::JoyStick* joystick_ois;

		GameManager* game_manager;

		int window_width;
		int window_height;

		std::string keyMap(const OIS::KeyEvent& e);
		//uint32 mouseMap(const OIS::MouseButtonID id);
		std::string joystickButtonMap(int button);
		int mouseMap(const OIS::MouseEvent& id);
		std::unordered_map<std::string,std::string> scriptKeyMap;
	public:
		InputManager(GameManager* gm);
		virtual ~InputManager();
		void checkForInput();
		bool keyPressed(const OIS::KeyEvent& e);
		bool keyReleased(const OIS::KeyEvent& e);
		void init();
		void free();
		bool buttonPressed(const OIS::JoyStickEvent& e, int button);
		bool buttonReleased(const OIS::JoyStickEvent& e, int button);
		bool sliderMoved(const OIS::JoyStickEvent& e, int index);
		bool povMoved(const OIS::JoyStickEvent& e, int index);
		bool vector3Moved(const OIS::JoyStickEvent& e, int index);
		bool axisMoved(const OIS::JoyStickEvent& e, int axis);
		std::string joystickAxisMap(int axis);
		bool mouseMoved(const OIS::MouseEvent& e);
		bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		void loadKeyScripts(std::vector<std::string> keys, std::vector<std::string> scripts);
};

#endif
