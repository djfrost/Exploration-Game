#if !defined COMPARECEGUIEVENT
#define COMPARECEGUIEVENT
#include <string>
#include "CEGUIEvent.h"
#include "CSC2110/Comparator.h"
class CompareCEGUIEvent :public Comparator<CEGUIEvent,std::string>{
  private:
  public:
    CompareCEGUIEvent();
    ~CompareCEGUIEvent();
    virtual int compare(CEGUIEvent* cegui_event_1, CEGUIEvent* cegui_event_2);
    virtual int compare(std::string* event_key, CEGUIEvent* cegui_event);
};

#endif
