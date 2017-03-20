#include "InputFunctionHandler.h"
#include "RenderManager.h"
#include "Ogre.h"
void func1(RenderManager* rm){
	rm->stopRendering();
};
void func2(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* mc = sm->getSceneNode("Fox_transform");
	mc->translate(mc->getOrientation() * Ogre::Vector3(0,0,0.2));
};
void func3(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* mc = sm->getSceneNode("Fox_transform");
	mc->translate(mc->getOrientation() * Ogre::Vector3(0.2,0,0));
};
void func4(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* mc = sm->getSceneNode("Fox_transform");
	mc->translate(mc->getOrientation() * Ogre::Vector3(0,0,-0.2));
};
void func5(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* mc = sm->getSceneNode("Fox_transform");
	mc->translate(mc->getOrientation() * Ogre::Vector3(-0.2	,0,0));
};
void func6(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* rotNode = sm->getSceneNode("camRot_node");
	Ogre::Quaternion Quat = rotNode->getOrientation();
	Ogre::Degree DegZ;
	DegZ = (2 * Ogre::Degree(Ogre::Math::ACos(rotNode->getOrientation().w)).valueDegrees());
	std::cout << DegZ.valueAngleUnits() << std::endl;
	if(DegZ.valueAngleUnits() <117.5){
		rotNode->pitch(Ogre::Degree(-0.8));
	}
};
void func7(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* rotNode = sm->getSceneNode("Fox_transform");
	rotNode->yaw(Ogre::Degree(0.8));
};
void func8(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* rotNode = sm->getSceneNode("camRot_node");
	Ogre::Quaternion Quat = rotNode->getOrientation();
	Ogre::Degree DegZ;
	DegZ = (2 * Ogre::Degree(Ogre::Math::ACos(rotNode->getOrientation().w)).valueDegrees());
	std::cout << DegZ.valueAngleUnits() << std::endl;
	if(DegZ.valueAngleUnits() > 90.05){
		rotNode->pitch(Ogre::Degree(0.8));
	}
};
void func9(RenderManager* rm){
	Ogre::SceneManager* sm = rm->getSceneManager();
	Ogre::SceneNode* rotNode = sm->getSceneNode("Fox_transform");
		rotNode->yaw(Ogre::Degree(-0.8));
};
InputFunctionHandler::InputFunctionHandler(RenderManager* render_manager){
	rm = render_manager;
	inputs.push_back("ESCAPE");
	inputs.push_back("W");
	inputs.push_back("A");
	inputs.push_back("S");
	inputs.push_back("D");
	inputs.push_back("UP");
	inputs.push_back("LEFT");
	inputs.push_back("DOWN");
	inputs.push_back("RIGHT");
	functions.push_back(func1);
	functions.push_back(func2);
	functions.push_back(func3);
	functions.push_back(func4);
	functions.push_back(func5);
	functions.push_back(func6);
	functions.push_back(func7);
	functions.push_back(func8);
	functions.push_back(func9);
	for(int i = 0; i < 9; i++){
		bools.push_back(false);
	}
}
InputFunctionHandler::~InputFunctionHandler(){
	rm = NULL;
}
void InputFunctionHandler::processInput(std::string game_key, bool released){
	for(int i = 0; i < inputs.size(); i++){
		if(inputs[i] == game_key){
			if(released){
				bools[i] = false;
			}
			else{
				bools[i] = true;
				functions[i](rm);
				i=inputs.size();
			}
		}
	}
}
void InputFunctionHandler::keyDownFunctions(){
	for(int i = 0; i < inputs.size(); i++){
		if(bools[i]){
			functions[i](rm);
		}
	}
}