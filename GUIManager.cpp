#include "GUIManager.h"
#include "RenderManager.h"
#include "CEGUIEvent.h"
#include "CompareCEGUIEvent.h"
#include "CSC2110/AVLTreeIterator.h"
#include <thread>
void GUIManager::doNothing(const CEGUI::EventArgs& e){

}
void GUIManager::playComboboxSample(const CEGUI::EventArgs& e){
	CEGUI::Combobox* combox_box = static_cast<CEGUI::Combobox*>(root_window->getChild("FrameWindow/Combobox"));
	CEGUI::String combo_box_text = combox_box->getText();
	render_manager->playAudio(combo_box_text.c_str());
}
void GUIManager::buttonEvent(const CEGUI::EventArgs& e){
	render_manager->changeLevel("level_1");
	std::cout << "Changed level" << std::endl;
}

void GUIManager::changeSongToUnchained(const CEGUI::EventArgs& e){
	render_manager->changeMainSong("bgsong");
	std::cout << "Wings Unchained button" << std::endl;
}
void GUIManager::changeSongToDawn(const CEGUI::EventArgs& e){
	render_manager->changeMainSong("torii-sound");
	std::cout << "Our Last Dawn button " << std::endl;
}
void GUIManager::loadHitSample(const CEGUI::EventArgs& e){
	std::cout << "Hit sample" << std::endl;
	render_manager->playSample("Hit");
}
void GUIManager::loadLaserSample(const CEGUI::EventArgs& e){
	std::cout << "Laser sample" << std::endl;
	render_manager->playSample("Laser");
}
void GUIManager::loadPowerupSample(const CEGUI::EventArgs& e){
	std::cout << "Hit Powerup" << std::endl;
	render_manager->playSample("Powerup");
}
void GUIManager::keyPressed(std::string game_key){
	if(game_key == "ENTER"){
		gui_context->injectKeyDown(CEGUI::Key::Return);
		gui_context->injectKeyUp(CEGUI::Key::Return);
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

void GUIManager::mousePressed(unsigned int x_click, unsigned int y_click, unsigned int mouse_button){
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

void GUIManager::mouseMoved(unsigned int x_click, unsigned int y_click,float x_rel,float y_rel){
	std::cout << "\n" << x_rel << "\n" << y_rel << std::endl;

	gui_context->injectMouseMove(x_rel, y_rel);
}
CEGUI::OgreRenderer* GUIManager::getRenderer(){
	static CEGUI::OgreRenderer& cegui_ogre_renderer = CEGUI::OgreRenderer::bootstrapSystem();
	return &cegui_ogre_renderer;
}

void GUIManager::createGUIContext(std::string scheme,std::string font,std::string cursor,std::string tooltip,std::string layout){

	CEGUI::SchemeManager::getSingleton().createFromFile(scheme + ".scheme");
	std::cout << "\n\n\n\nInside of gui context" << std::endl;
	CEGUI::FontManager::getSingleton().createFromFile(font + ".font");
	CEGUI::System& system = CEGUI::System::getSingleton();
	std::cout << "\n\n\n\ncreating gui context" << std::endl;
	gui_context = &system.createGUIContext(cegui_ogre_renderer->getDefaultRenderTarget());
	gui_context->setDefaultFont(font);
	gui_context->getMouseCursor().setDefaultImage(cursor);
	gui_context->setDefaultTooltipType(tooltip);
	std::cout << "\n\n\n\nLoading Layout file" << std::endl;
	CEGUI::WindowManager* wmgr = &CEGUI::WindowManager::getSingleton();
	root_window = wmgr->loadLayoutFromFile(layout + ".layout");
	gui_context->setRootWindow(root_window);
}

void GUIManager::processCombobox(std::string name_str, std::string image_str, std::string * items, int num_items){
	CEGUI::Combobox* combo_box = static_cast<CEGUI::Combobox*>(root_window->getChild(name_str));
	combo_box->setReadOnly(true);
	for(int i = 1;i <= num_items; i++ ){
		CEGUI::ListboxTextItem* combo_item = new CEGUI::ListboxTextItem(items[i-1],1);
		combo_item->setSelectionBrushImage(image_str);
		combo_box->setText(combo_item->getText());
		if(i == 1){
			combo_item->setSelected(true);
			combo_box->setText(combo_item->getText());
		}
	}
}

void GUIManager::processEvent(std::string type_str, std::string name_str, std::string event){
	int eventNum = 0;
	std::cout << "Event! From " << name_str << std::endl;
	if(event == "LevelButton"){
		eventNum = 0;
	}
	else if(event == "WingsUnchained"){
		eventNum = 1;
	}
	else if(event == "OurLastDawn"){
		eventNum = 2;
	}
	else if(event == "Hit"){
		eventNum = 3;
	}
	else if(event == "Laser"){
		eventNum = 4;
	}
	else if(event == "Powerup"){
		eventNum = 5;
	}
	CEGUIEvent* cegui_event = events[eventNum];
	cegui_function_ptr event_function_ptr = cegui_event->getFunctionPtr();

	if(type_str == "button"){
		if(eventNum == 0){
			CEGUI::PushButton* push_button = static_cast<CEGUI::PushButton*>(root_window->getChild(name_str));
			push_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(event_function_ptr, this));
			std::cout << "Successfully got event" << std::endl;
		}
		else{
			CEGUI::PushButton* push_button = static_cast<CEGUI::PushButton*>(root_window->getChild("FrameWindow")->getChild(name_str));
			push_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(event_function_ptr, this));
			std::cout << "Successfully got event 2" << std::endl;
		}
	}
}
void GUIManager::destroyGUIContext(){
	CEGUI::System& system = CEGUI::System::getSingleton();
	if(gui_context != NULL){
		system.destroyGUIContext(*gui_context);
		gui_context = NULL;
	}
	CEGUI::SchemeManager::getSingleton().destroyAll();
	CEGUI::FontManager::getSingleton().destroyAll();
}

GUIManager::GUIManager(RenderManager* rm){
		root_window = NULL;
		gui_context = NULL;
		render_manager = rm;
		cegui_ogre_renderer = getRenderer();
		cegui_function_ptr button_func_ptr = &GUIManager::buttonEvent;
		CEGUIEvent* cegui_event = new CEGUIEvent("buttonEvent", button_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr unchained_sample_func_ptr = &GUIManager::changeSongToUnchained;
		cegui_event = new CEGUIEvent("WingsUnchained", unchained_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr dawn_sample_func_ptr = &GUIManager::changeSongToDawn;
		cegui_event = new CEGUIEvent("OurLastDawn", dawn_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr hit_sample_func_ptr = &GUIManager::loadHitSample;
		cegui_event = new CEGUIEvent("Hit", hit_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr laser_sample_func_ptr = &GUIManager::loadLaserSample;
		cegui_event = new CEGUIEvent("Laser", laser_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr powerup_sample_func_ptr = &GUIManager::loadPowerupSample;
		cegui_event = new CEGUIEvent("Powerup", powerup_sample_func_ptr);
		events.push_back(cegui_event);
}

GUIManager::~GUIManager(){
	cegui_ogre_renderer = NULL;
	for(int i = 0; i < events.size(); i++){
		delete events[i];
		events[i] = NULL;
	}
}

void GUIManager::unloadLevel(){
	if(gui_context != NULL){
		destroyGUIContext();
		gui_context = NULL;
		for(int i = 0; i < events.size(); i++){
			delete events[i];
		}
		events.clear();
		cegui_function_ptr button_func_ptr = &GUIManager::buttonEvent;
		CEGUIEvent* cegui_event = new CEGUIEvent("buttonEvent", button_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr unchained_sample_func_ptr = &GUIManager::changeSongToUnchained;
		cegui_event = new CEGUIEvent("WingsUnchained", unchained_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr dawn_sample_func_ptr = &GUIManager::changeSongToDawn;
		cegui_event = new CEGUIEvent("OurLastDawn", dawn_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr hit_sample_func_ptr = &GUIManager::loadHitSample;
		cegui_event = new CEGUIEvent("Hit", hit_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr laser_sample_func_ptr = &GUIManager::loadLaserSample;
		cegui_event = new CEGUIEvent("Laser", laser_sample_func_ptr);
		events.push_back(cegui_event);
		cegui_function_ptr powerup_sample_func_ptr = &GUIManager::loadPowerupSample;
		cegui_event = new CEGUIEvent("Powerup", powerup_sample_func_ptr);
		events.push_back(cegui_event);
	}
}

void GUIManager::loadLevel(std::string level_name, std::string scheme, std::string font, std::string cursor, std::string tooltip, std::string layout){
	if(gui_context == NULL){
		CEGUI::ImageManager::setImagesetDefaultResourceGroup(level_name);
		CEGUI::Scheme::setDefaultResourceGroup(level_name);
		CEGUI::Font::setDefaultResourceGroup(level_name);
		CEGUI::WidgetLookManager::setDefaultResourceGroup(level_name);
		CEGUI::WindowManager::setDefaultResourceGroup(level_name);
		createGUIContext(scheme, font, cursor, tooltip, layout);
	}
}
