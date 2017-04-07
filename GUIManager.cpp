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

void GUIManager::mouseMoved(uin32 x_click, uint32 y_click, int x_rel, int y_rel){
	gui_context->injectMouseMove(x_rel, y_rel);
}
CEGUII::OgreRederer* GUIManager::getRenderer(){
	static CEGUI::OgreRenderer& cegui_ogre_renderer = CEGUI::OgreRenderer::bootstrapSystem();
	return cegui_ogre_renderer;
}

void GUIManager::createGUIContext(){
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::FontManager::getSingleton().createFromFile("GreatVibes-16.font");
	CEGUI::System& system = CEGUI::System::getSingleton();
	gui_context = &system.createGUIContext(cegui_ogre_renderer->getDefaultRenderTarget());
	gui_context->setDefaultFont("GreatVibes-16");
	gui_context->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	gui_context->setDefaultTooltipType("TaharezLook/Tooltip");
	CEGUI::WindowManager* wmgr = &CEGUI::WindowManager::getSingleton();
	root_window = wmgr->loadLayoutFromFile("MyDemo.layout");
	gui_context->setRootWindow(root_window);

	CEGUI::PushButton* push_button = static_cast<CEGUI::PushButton*>(root_window->getChild("FrameWindow/Button"));
	push_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::buttonEvent, this));
}

void GUIManager::processEvent(std::string type_str, std::string name_str, std::string procedure_str){
	void (GUIManager::*event_function_ptr)(const CEGUI::EventArgs&);
	if(type_str == "edit_box"){
		CEGUI::PushButton* push_button = static_cast<CEGUI::PushButton*>(root_window->getChild("FrameWindow/Button"));
		if(procedure_str == "buttonEvent"){
			event_function_ptr = &GUIManager::buttonEvent;
			push_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscrier(&GUIManager::buttonEvent, this));
		}
		else if(type_str == "edit_box"){
			CEGUI::Editbox* edit_box = static_cast<CEGUI::Editbox*>(root_window->getChild(name_str));
			if(procedure_str == "editboxEvent"){
				event_function_ptr = &GUIManager::editboxEvent;
				edit_box->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&GUIManager::buttonEvent, this))
			}
		}
		push_button->
	}
}
void GUIManager::destroyGUIContext(){
	CEGUI::System& system = CEGUI::System::getSingleton();
	if(gui_context != NULL){
		system.destroyGUIContext(*gui_context);
		gui_context = NULL;
	}
}
