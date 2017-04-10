#if !defined GUI_MANAGER
#define GUI_MANAGER
#include "CEGUI/CEGUI.h"
#include "CEGUIEvent.h"
#include "CompareCEGUIEvent.h"
#include "CEGUI/RendererModules/Ogre/Renderer.h"
#include "CSC2110/TableAVL.h"
class RenderManager;


class GUIManager{
  private:
    static CEGUI::OgreRenderer* getRenderer();
    CEGUI::OgreRenderer* cegui_ogre_renderer;
    RenderManager* render_manager;
    void createGUIContext();
    CEGUI::GUIContext* gui_context;
    CEGUI::Window* root_window;
    void destroyGUIContext();
    std::vector<CEGUIEvent*> events;
    // TableAVL<CEGUIEvent*, std::string>* event_table;
    void doNothing(const CEGUI::EventArgs& e);
    void playComboboxSample(const CEGUI::EventArgs& e);
    //CompareCEGUIEvent* comparator; //possibly CEGUIEvent* not CEGUIEvent
  public:
      GUIManager(RenderManager* rm);
      virtual ~GUIManager();

      void loadLevel(std::string level_name, std::string scheme, std::string font, std::string cursor, std::string tooltip, std::string layout);
      void unloadLevel();
      void keyPressed(std::string game_key);
      void createGUIContext(std::string scheme,std::string font,std::string cursor,std::string tooltip,std::string layout);
      void mouseMoved(unsigned int mouse_x, unsigned int mouse_y,float mouse_rel_x,float mouse_rel_y);
      void mousePressed(unsigned int mouse_x, unsigned int mouse_y, unsigned int mouse_button);
      void processEvent(std::string type_str, std::string name_str, std::string event);
      void buttonEvent(const CEGUI::EventArgs& e);
      void processCombobox(std::string name_str, std::string image_str, std::string * items, int num_items);
};

#endif
