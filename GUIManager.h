#if !defined GUI_MANAGER
#define GUI_MANAGER
#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/Ogre/Renderer.h"

class RenderManager;

class GUIManger{
  private:
    RenderManager* render_manager;
    static CEGUI::OgreRenderer* getRenderer();
    CEGUI::OgreRenderer* cegui_ogre_renderer;

    void createGUIContext();
    CEGUI::GUIContext* gui_context;
    CEGUI::Window* root_window;
    void destroyGUIContext();
  public:
      GUIManager(RenderManager* render_manager);
      virtual ~GUIManager();

      void loadLevel(std::string level_name);
      void unloadLevel();
      void keyPressed(std::string game_key);
      void mouseMoved(uint32 mouse_x, uint32 mouse_y, int mouse_rel_x, int mouse_rel_y);
      void mousePressed(uint32 mouse_x, uint32 mouse_y, uint32 mouse_button);

      void buttonEvent(const CEGUI::EventArgs& e);
};

#endif
