#include "RenderListener.h"
RenderListener::RenderListener(RenderManager* render_manager){
	rm = render_manager;
	render = true;
}
RenderListener::~RenderListener(){
	rm = NULL;
}
void RenderListener::stopRendering(){
	render = false;
}
bool RenderListener::getRenderStatus(){
	return render;
}
void RenderListener::startRendering(){
	render = true;
}
RenderManager* RenderListener::getRenderManager(){
	return rm;
}