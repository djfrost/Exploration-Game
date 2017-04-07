#include "GUIManager.h"

void GUIManager::buttonEvent(const CEGUI::EventArgs& e){
	std::cout << "Button_Clicked!!!" << std::endl;
}
void GUIManager::keyPressed(std::string game_key){
	gui_context->injectChar((char)game_key);
}
void GUIManager::mousePressed(uint32 x_click, uint32 y_click, uint32 mouse_button){
	CEGUI::MouseButton mouse_enum = CEGUI::NoButton;
	if(mouse_button == 1){
		mouse_enum = CEGUI::LeftButton;
	}
	else if(mouse_button == 2){
		mouse_enum = CEGUI::RightButton;
	}
	gui_context->injectMouseButtonDown(mouse_enum);
	gui_context->injectMouseButtonUp(mouse_enum);
}
