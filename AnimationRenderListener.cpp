#include "AnimationRenderListener.h"
#include "RenderManager.h"
AnimationRenderListener::AnimationRenderListener(RenderManager* rm){
	render_manager = rm;
	render = true;
}
AnimationRenderListener::~AnimationRenderListener(){
	render_manager = NULL;
}
bool AnimationRenderListener::getRenderStatus(){
	return render;
}
RenderManager* AnimationRenderListener::getRenderManager(){
	return render_manager;
}
void AnimationRenderListener::stopRendering(){
	render = false;
}

bool AnimationRenderListener::frameStarted(const Ogre::FrameEvent& event){
	float timeStep = event.timeSinceLastFrame;
	getRenderManager()->processAnimations(timeStep);
	return getRenderStatus();
}

bool AnimationRenderListener::frameRenderingQueued(const Ogre::FrameEvent& event){}
bool AnimationRenderListener::frameEnded(const Ogre::FrameEvent& event){}