#include "CEGUIEvent.h"

CEGUIEvent::CEGUIEvent(std::string ekey, cegui_function_ptr event_ptr){
  event_function_ptr = event_ptr;
  event_key = ekey;
}
CEGUIEvent::~CEGUIEvent(){}
std::string* CEGUIEvent::getEventKey(){
  return &event_key;
}
std::string& CEGUIEvent::getEventScript(){
}
cegui_function_ptr CEGUIEvent::getFunctionPtr(){
  return event_function_ptr;
}
void CEGUIEvent::setEventScript(std::string& script){

}
