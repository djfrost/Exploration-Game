#include "AudioResource.h"
#include "GameManager.h"

#include <cstdlib>

std::string AudioResource::getAudioResourceName(){
	return audio_resource_name;
}
AudioResourceType AudioResource::getAudioResourceType(){
	return audio_resource_type;
}
AudioResource::AudioResource(std::string level_name, std::string audio_file_name, std::string audio_name, AudioResourceType audio_type) : GameResource(level_name, audio_file_name, AUDIO){
	audio_resource_type = audio_type;
	audio_resource_name = audio_name;
}
AudioResource::~AudioResource(){}
void AudioResource::load(GameManager* gm){
	ar_info = gm->createAudioResourceInfo();
	if(audio_resource_type == SAMPLE){
		gm->loadSampleAudioResource(getResourceFileName(), ar_info);
	}
	else{
		gm->loadStreamAudioResource(getResourceFileName(), ar_info);
	}
}
void AudioResource::unload(GameManager* gm){
	if(audio_resource_type == SAMPLE){
		gm->unloadSampleAudioResource(ar_info);
	}
	else{
		gm->unloadStreamAudioResource(ar_info);
	}
	free(ar_info);
}
AudioResourceInfo* AudioResource::getAudioResourceInfo(){
	return ar_info;
}