#include "GUIManager.h"
#include "RenderManager.h"
#include "CEGUIEvent.h"
#include "CompareCEGUIEvent.h"
#include "CSC2110/AVLTreeIterator.h"

void GUIManager::buttonEventCallback(const CEGUI::EventArgs& e){
	std::string button_event_calback("ButtonEvent");
	CEGUIEvent* cegui_event = event_table->tableRetrieve(&button_event_calback);
	std::string& script_name = cegui_event->getEventScript();
	
	CEGUI::Combobox* combo_box = static_cast<CEGUI::Combobox*>(root_window->getChild("FrameWindow"));
	CEGUI::String combo_box_text = combo_box->getText();
	std::string combo_box_str(combo_box.c_str());
	render_manager->buttonEventCallback(combo_box_str, script_name);
}

void GUIManager::keyPressed(std::string game_key){
	if(game_key == "ENTER"){
		gui_context->injectKeyDown(CEGUI::Key::Return);
		gui_context->injectKeyUp(CEGUI::Key::Return)
	}
	else if(game_key == "SPACE"){
		gui_context->injectChar(' ');
	}
	else if(game_key == "BACK"){
		gui_context->injectKeyDown(CEGUI::Key::Backspace);
		gui_context->injectKeyUp(CEGUI::Key::Backspace);
	}
	else{
		gui_context->injectChar((char)game_key[0]);
	}
}
void GUIManager::MousePressed(uint32 x_click, uint32 y_click, uint32 mouse_button){
}
void GUIManager::MousePressed(uint32 x_click, uint32 y_click, uint32 mouse_button){
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
void GUIManager::mouseMoved(uint32 x_click, uint32 y_click, int x_rel, int y_rel){
	gui_context->injectMouseOver(x_rel, y_rel);
}
CEGUI::OgreRenderer* GUIManager::getRenderer(){
	static CEGUI::OgreRenderer* cegui_ogre_renderer = CEGUI::OgreRenderer::bootstrapSystem();
	return &cegui_ogre_renderer;
}
void GUIManager::createGUIContext(std::string scheme_str, std::string font_str, std::string cursor_str, std::string tooltip_str, std::string layout_str){
	CEGUI::SchemeManager::getSingleton().createFromFile(scheme_str + ".scheme");
	CEGUI::FontManager::getSingleton().createFromFile(font_str + ".font");
	CEGUI::System& system = CEGUI::System.getSingleton();
	gui_context = &system.createGUIContext(cegui_ogre_renderer->getDefaultRenderTarget());
	gui_context->setDefaultFont(font_str);
	gui_context->getMouseCursor().setDefaultImage(cursor_str);
	gui_context->setDefaultTooltipType(tooltip_str);
	
	CEGUI::WindowManager* wmgr = &CEGUI::WindowManager::getSingleton();
	root_window = wmgr->loadLayoutFromFile(layout_str + "layout");
	gui_context->setRootWindow(root_window);
}

void GUIManager::processCombobox(std::string& name_str, std::string& image_str, std::string* items, int num_items){
	CEGUI::ComboBox* combo_box = static_cast<CEGUI::Combobox*>(root_window->getCh);
	combo_box->setReadOnly(true);
	for(int i = 1;i <= num_items; i++){
		CEGUI::ListBoxItem* combo_item = new CEGUI::ListBoxTextItem(items[i-1],1);
		combo_item->setSelectionBrushImage(image_str);
		combo_box->addItem(combo_item);
		
		if(i==1){
			combo_item->setSelected(true);
			combo_box->setText(combo_item->getText());
		}
	}
}

void GUIManager::processEvent(std::string& event_type_str, std::string& component_name_str, std::string& script_str){
	CEGUIEvent* cegui_event = event_table->tableRetrieve(&event_type_str);
	if(cegui_event != NULL){
		cegui_event->setEventScript(script_str);
		cegui_function_ptr event_function_ptr = cegui_event->getFunctionPtr();
		CEGUI::PushButton* push_button = static_cast<CEGUI::PushButton*>(root_window->getChild(component_name_str);
		push_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(event_function_ptr, this));
	}
	/*
	else if(type_str == "edit_box"){
		CEGUI::Editbox* edit_box = static_cast<CEGUI::Editbox*>(root_window->getChild(name_str);
		if(procedure_str == "editboxEvent"){
			event_function_ptr = &GUIManager::editboxEvent;
			edit_box->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscribed);
		}
	}
	*/
}

void GUIManager::destroyGUIContext(){
	CEGUI::System& system = CEGUI::System::getSingleton();
	if(gui_context != NULL){
		system.destroyGUIContext(*gui_context);
		gui_context = NULL
	}
	CEGUI::SchemeManager::getSingleton().destroyAll();
	CEGUI::FontManager::getSingleton().destroyAll();
}

GUIManager::GUIManager(RenderManager* rm){
	render_manager = rm;
	root_window = NULL;
	gui_context = NULL;
	
	cegui_ogre_renderer = getRenderer();
	comparator = new CompareCEGUIEvent();
	event_table = TableABL<CEGUIEvent, std::string>(comparator);
	
	cegui_function_ptr button_event_calback_func_ptr = &GUIManager::buttonEventCallback;
	std::string button_event_calback("ButtonEvent");
	CEGUIEvent* cegui_event = new CEGUIEvent(button_event_calback, button_event_calback_func_ptr);
	event_table->tableInsert(cegui_event);
}

GUIManager::~GUIManager(){
	cegui_ogre_renderer = NULL;
	AVLTreeIterator<CEGUIEvent>* event_iter = event_table->tableIterator();
	while(event_iter->hasNext()){
		CEGUIEvent* cegui_event = event_iter->next();
		delete cegui_event;
	}
	delete event_iter;
	delete event_table;
	event_table = NULL;
	
	delete comparator;
	comparator = NULL;
}

void GUIManager::unloadLevel(){
	if(gui_context != NULL){
		destroyGUIContext();
		gui_context = NULL
	}
}

void GUIManager::loadLevel(std::string level_name, std::string scheme, std::string font, std::string cursor, std::string tooltip, std::string layout){
	if(gui_context != NULL){
		CEGUI::ImageManager::setImagesetDefaultResourceGroup(level_name);
		CEGUI::Scheme::setDefaultResourceGroup(level_name);
		CEGUI::Font::setDefaultResourceGroup(level_name);
		CEGUI::WidgetLookmanager::setDefaultResourceGroup(level_name);
		CEGUI::WindowManager::setDefaultResourceGroup(level_name);
		CEGUI::ScriptModule::setDefaultResourceGroup(level_name);
		
		createGUIContext(scheme, font, cursor, tooltip, layout)
	}
}