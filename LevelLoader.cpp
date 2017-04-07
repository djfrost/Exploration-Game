#include "LevelLoader.h"
#include <boost/foreach.hpp>
#include "GameManager.h"
#include <sstream>
LevelLoader::LevelLoader(GameManager* gameManager, std::string fileName){
	gm = gameManager;
	ReadResources(fileName);
	//Now that we have the ptree from the json file, we want to load the first level
	std::string levelOne = pt.get<std::string>("first_level");
	currScene = "";
	nextScene = "";
	LoadLevel(levelOne);
}

LevelLoader::~LevelLoader(){

}

void LevelLoader::ReadResources(std::string fileName){
	std::ifstream jsonFile(fileName.c_str());
	read_json(jsonFile, pt);
	std::string output;
	jsonFile.close();
}

void LevelLoader::LoadLevel(std::string levelName)
{
	nextScene = levelName;
	ptree levelTree;
	gm->initialiseNewScene();
	std::string level = "levels." + levelName;
	levelTree = pt.get_child(level);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, levelTree.get_child("path")){
		gm->addPath(v.second.data(), levelName);
	}
	ptree meshTree = levelTree.get_child("meshes");
	std::vector<std::string> meshes;
	std::vector<std::string> meshFiles;
	std::vector< std::vector< float > > transforms;
	std::vector< std::vector< float > > rotates;
	std::vector< std::vector< float > > scales;
	std::vector<float> angle;
	std::vector<std::string> defaultAnims;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, meshTree.get_child("meshNames")){
		BOOST_FOREACH(boost::property_tree::ptree::value_type &t, meshTree.get_child(v.second.data())){
			std::string type = t.first.data();
			if(type == "file")
			{
				meshFiles.push_back(t.second.data());
			}
			else if(type == "transform"){
				std::string transNotParsed = t.second.data();
				transforms.push_back(parseMultF(transNotParsed));
			}
			else if(type == "rotate"){
				std::string rotNotParsed = t.second.data();
				rotates.push_back(parseMultF(rotNotParsed));
			}
			else if(type == "scale"){
				std::string scaleNotParsed = t.second.data();
				scales.push_back(parseMultF(scaleNotParsed));
			}
			else if(type == "angle"){
				std::string anglestr = t.second.data();
				angle.push_back(stof(anglestr));
			}
			else if(type == "defaultAnim"){
				std::string anim = t.second.data();
				defaultAnims.push_back(anim);
				std::cout << anim << std::endl << std::endl << std::endl << std::endl;
			}
		}
		meshes.push_back(v.second.data());
	}
	ptree cameraTree = levelTree.get_child("Cameras");
	std::vector< std::vector < float > > positions;
	std::vector< std::vector < float > > lookAts;
	std::vector<float> nearclips;
	std::vector<float> farclips;
	std::vector<std::string> parents;
	std::vector<float> camRots;
	std::vector<float> camAngle;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, cameraTree.get_child("Names")){
		BOOST_FOREACH(boost::property_tree::ptree::value_type &t, cameraTree.get_child(v.second.data())){
			std::string type = t.first.data();
			if(type == "position")
			{
				std::string posNotParsed = t.second.data();
				positions.push_back(parseMultF(posNotParsed));
			}
			else if(type == "lookat"){
				std::string laNotParsed = t.second.data();
				lookAts.push_back(parseMultF(laNotParsed));
			}
			else if(type == "nearclip"){
				nearclips.push_back(stof(t.second.data()));
			}
			else if(type == "farclip"){
				farclips.push_back(stof(t.second.data()));
			}
			else if(type == "rotation"){
				std::string rotNotParsed = t.second.data();
				camRots = parseMultF(rotNotParsed);
			}
			else if(type == "angle"){
				camAngle.push_back(stof(t.second.data()));
			}
			else if(type == "childOf"){
				parents.push_back(t.second.data());
			}
		}
	}
	ptree lightTree = levelTree.get_child("Lights");
	std::vector<std::string> names;
	std::vector<float> types;
	std::vector< std::vector < float > > colors;
	std::vector< std::vector < float > > directions;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, lightTree.get_child("Names")){
		std::string name = v.second.data();
		names.push_back(name);
		BOOST_FOREACH(boost::property_tree::ptree::value_type &t, lightTree.get_child(v.second.data())){
			std::string type = t.first.data();
			if(type == "type"){
				types.push_back(stof(t.second.data()));
			}
			else if(type == "color"){
				std::string colNotParsed = t.second.data();
				colors.push_back(parseMultF(colNotParsed));
			}
			else if(type == "direction"){
				std::string dirNotParsed = t.second.data();
				directions.push_back(parseMultF(dirNotParsed));
			}
		}
	}
	ptree animTree = meshTree.get_child("Animations");
	std::vector<std::string> objects;
	std::vector<std::string> typesAnims;
	std::vector< std::vector < float > > values;
	std::vector< std::vector < float > > axis;
	std::vector< std::vector < float > > timeSteps;
	std::vector< std::vector < float > > start;
	std::vector< std::vector < float > > begin;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, animTree.get_child("AnimNames")){
		BOOST_FOREACH(boost::property_tree::ptree::value_type &t, animTree.get_child(v.second.data())){
			std::string type = t.first.data();
			if(type == "object")
			{
				objects.push_back(t.second.data());
			}
			else if(type == "type"){
				typesAnims.push_back(t.second.data());
			}
			else if(type == "value"){
				std::string valNotParsed = t.second.data();
				values.push_back(parseMultF(valNotParsed));
			}
			else if(type == "axis"){
				std::string axNotParsed = t.second.data();
				axis.push_back(parseMultF(axNotParsed));
			}
			else if(type == "timestep"){
				std::string timeNotParsed = t.second.data();
				timeSteps.push_back(parseMultF(timeNotParsed));
			}
			else if(type == "start"){
				std::string startNotParsed = t.second.data();
				start.push_back(parseMultF(startNotParsed));
			}
			else if(type == "begin"){
				std::string begNotParsed = t.second.data();
				begin.push_back(parseMultF(begNotParsed));
			}
		}
	}
	ptree audioTree = levelTree.get_child("SFX");
	std::vector<std::string> audioTypes;
	std::vector<std::string> audioFiles;
	std::vector<int> audioRepeats;
	std::vector<std::string> audioNames;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, audioTree.get_child("Names")){
		BOOST_FOREACH(boost::property_tree::ptree::value_type &t, audioTree.get_child(v.second.data())){
			std::string type = t.first.data();
			if(type == "type")
			{
				audioTypes.push_back(t.second.data());
			}
			else if(type == "file"){
				audioFiles.push_back(t.second.data());
			}
			else if(type == "repeats"){
				std::string valNotParsed = t.second.data();
				audioRepeats.push_back(stof(valNotParsed));
			}
			else if(type == "name"){
				audioNames.push_back(t.second.data());
			}
		}
	}
	std::string skybox = levelTree.get<std::string>("SkyMap");
	// unload last scene load next scene
	gm->loadLights(names,types, colors, directions);
	gm->loadScene(nextScene, currScene, meshes, meshFiles, transforms, rotates, angle, scales, defaultAnims);
	gm->loadCameras(positions, lookAts, nearclips, farclips, camRots, camAngle, parents);
	gm->loadSkyBox(skybox);
	handleAudioResources(audioTypes, audioFiles, audioRepeats, audioNames, levelName);
	//gm->processAnims(objects, typesAnims, values, axis, timeSteps, start, begin);
	currScene = levelName;
	nextScene = "";
}
std::vector<float> LevelLoader::parseMultF(std::string floats){
	std::vector<float> floatVec;
	std::stringstream ss;
	ss.str(floats);
	std::string tempFloat;
	while(std::getline(ss, tempFloat, ',')){
		floatVec.push_back(stof(tempFloat));
	}
	return floatVec;
}
void LevelLoader::unLoadCurrLevel(){
	for(int i = 0; i < currentLevelAudio.size(); i++){
		delete currentLevelAudio[i];
	}
	currentLevelAudio.clear();
}
void LevelLoader::handleAudioResources(std::vector<std::string> types, std::vector<std::string> files, std::vector<int> repeats, std::vector<std::string> names, std::string level){
	for(int i = 0; i < types.size(); i++){
		AudioResource* ar;
		if("stream" == types[i]){
			ar = new AudioResource(level, files[i], names[i], STREAM, gm);
			ar->load();
			gm->loadStreamAudioResource(files[i], ar->getAudioResourceInfo());
			gm->playAudio(ar,repeats[i]);
		}
		else{
			ar = new AudioResource(level, files[i], names[i], SAMPLE, gm);
		}
		currentLevelAudio.push_back(ar);
	}
}
