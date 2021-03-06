#include "AudioManager.h"
#include "AudioPlayer.h"
#include "AudioResource.h"
#include "GameManager.h"
#include <iostream>
struct AudioResourceInfo {
	HSAMPLE sample_data;
	HSAMPLE stream_data;
	HSAMPLE channel_data;
};

AudioResourceInfo* AudioManager::createAudioResourceInfo(){
	AudioResourceInfo* ar  = (AudioResourceInfo*) malloc(sizeof(AudioResourceInfo));
	ar->sample_data = 0;
	ar->channel_data = 0;
	return ar;
}
void AudioManager::unloadSampleAudioResource(AudioResourceInfo* ar_info){
	if(!BASS_ChannelIsActive(ar_info->channel_data)==BASS_ACTIVE_STOPPED) {
		BASS_ChannelStop(ar_info->channel_data);
	}

	BASS_SampleFree(ar_info->sample_data);
	ar_info->sample_data = 0;
	ar_info->channel_data = 0;
}
void AudioManager::unloadStreamAudioResource(AudioResourceInfo* ar_info){
	if(!BASS_ChannelIsActive(ar_info->channel_data)==BASS_ACTIVE_STOPPED) {
		BASS_ChannelStop(ar_info->channel_data);
	}
	BASS_StreamFree(ar_info->stream_data);
	ar_info->sample_data = 0;
	ar_info->channel_data = 0;
}
void AudioManager::loadSampleAudioResource(std::string file_name, AudioResourceInfo* ar_info){
	ar_info->sample_data = BASS_SampleLoad(FALSE, file_name.c_str(), 0, 0, 1, 0);
	if(ar_info->sample_data) {
		ar_info->channel_data = BASS_SampleGetChannel(ar_info->sample_data, false);
	}
	else{
		ASSERT_LOG(false, "Sample Audio Resource Initialization Error" + file_name);
	}
}
void AudioManager::loadStreamAudioResource(std::string file_name, AudioResourceInfo* ar_info){
	ar_info->stream_data = BASS_StreamCreateFile(FALSE, file_name.c_str(), 0, 0, 0);
	if(ar_info->stream_data) {
		std::cout << "\n\n\n\n\n\n\n" <<file_name.c_str() << "\nSetting channel_data " << std::endl;
		ar_info->channel_data = ar_info->stream_data;
	}
	else{
		ASSERT_LOG(false, "Stream Audio Resource Initialization Error" + file_name);
	}
}

AudioManager::AudioManager(GameManager* gm){
	game_manager = gm;
	audio_players = new ListArray<AudioPlayer>();
	init();
}
AudioManager::~AudioManager(){
	game_manager = NULL;
	free();
	ListArrayIterator<AudioPlayer>* iter = audio_players->iterator();
	while(iter->hasNext()) {
		AudioPlayer* ap = iter->next();
		delete ap;
	}
	delete iter;
	delete audio_players;
	audio_players = NULL;
}
void AudioManager::init(int Device, DWORD SampleRate, DWORD flags, HWND win){
	//default values
	//int Device = 1; DWORD SampleRate = 44100, DWORD flags = 0, HWND win = 0
	BOOL bassActive = BASS_Init(-1, SampleRate, flags, win, NULL);
	if(!bassActive) {
		ASSERT_LOG(false, "Audio Manager Initialization Error");
	}
	std::stringstream DeviceStringStream;
	if(BASS_GetDeviceInfo(Device, &device_info)) {
		DeviceStringStream << "Audio Device Info. Name: " << device_info.name << "Driver: " << device_info.driver;
		std::string device_info_str = DeviceStringStream.str();
		std::cout << device_info_str << std::endl;
	}
	else{
		BASS_Free(); //clean up because of failed initialization
		ASSERT_LOG(false, "Audio Manager Initialization Problem");
	}
}
void AudioManager::free(){
	BASS_Free();
}
void AudioManager::playAudio(AudioResource* ar, uint32 num_repeats){
	if(audio_players->size() == 2) {return;}
	AudioPlayer* ap = new AudioPlayer(ar, num_repeats);
	addAudioPlayer(ap);
	AudioResource* audio_resource = ap->getAudioResource();
	AudioResourceInfo* ar_info = audio_resource->getAudioResourceInfo();
	std::cout << "\n\n\n\nPlaying audio from " << audio_resource->getResourceFileName() << std::endl;
	BASS_ChannelPlay(ar_info->channel_data, true);
}
void AudioManager::playSample(AudioResource* ar){
	ar->load();
	playAudio(ar, 0);
}
void AudioManager::addAudioPlayer(AudioPlayer* audio_player){
	audio_players->add(audio_player);
}
void AudioManager::updateAudio(float time_step){
	int num_players = audio_players->size();
	for(int i = 1; i <= num_players; i++) {
		AudioPlayer* audio_player = audio_players->get(i);
		AudioResource* audio_resource = audio_player->getAudioResource();
		AudioResourceInfo* ar_info = audio_resource->getAudioResourceInfo();
		if(BASS_ChannelIsActive(ar_info->channel_data) == BASS_ACTIVE_STOPPED) {
			if(audio_player->isAudioPlayerDone()) {
				audio_players->remove(i);
				delete audio_player;
				i--;
				num_players--;
			}
			else{
				audio_player->incRepeatCount();
				BASS_ChannelPlay(ar_info->channel_data, false);
			}
		}
	}
}
void AudioManager::unloadLevel(){
	ListArrayIterator<AudioPlayer>* iter = audio_players->iterator();
	while(iter->hasNext()) {
		AudioPlayer* ap = iter->next();
		if(ap->getAudioResource()->getAudioResourceType() == STREAM){
			unloadStreamAudioResource(ap->getAudioResource()->getAudioResourceInfo());
		}
		else{
			unloadSampleAudioResource(ap->getAudioResource()->getAudioResourceInfo());
		}
		delete ap;
	}
	delete iter;
	audio_players->removeAll();
}
void AudioManager::stopAudio(std::string name){
	for(int i = 0; i < audio_players->size(); i++){
		AudioPlayer* ap = audio_players->get(i);
		if(ap->getAudioResource()->getAudioResourceName() == name){
			if(ap->getAudioResource()->getAudioResourceType() == STREAM){
				unloadStreamAudioResource(ap->getAudioResource()->getAudioResourceInfo());
			}
			else{
				unloadSampleAudioResource(ap->getAudioResource()->getAudioResourceInfo());
			}
		}
		audio_players->remove(i);
		delete ap;
		i--;
	}
}
void AudioManager::unloadMainStream(){
	std::cout << "Inside unloadMainStream" << std::endl;
	for(int i = 0; i < audio_players->size(); i++){
		AudioPlayer* ap = audio_players->get(i+1);
		std::cout << "Before if " <<  audio_players->size() << std::endl;
		if(ap->getAudioResource()->getAudioResourceType() == STREAM){
			std::cout << "Unloading stream" << std::endl;
			unloadStreamAudioResource(ap->getAudioResource()->getAudioResourceInfo());
			std::cout << "Stream unloaded" << std::endl;
		}
		audio_players->remove(i+1);
		delete ap;
		i--;
	}
}
