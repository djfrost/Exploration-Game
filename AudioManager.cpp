#include "AudioManager.h"
#include "AudioPlayer.h"
#include "AudioResource.h"
#include "GameManager.h"

struct AudioResourceInfo{
	HSAMPLE sample_data;
	HSAMPLE stream_data;
	HSAMPLE channel_data;
}

AudioResourceInfo* AudioManager::createAudioResourceInfo(){
	AudioResourceInfo* ar  = (AudioResourceInfo*) malloc(sizeof(AudioResourceInfo));
	ar->sample_data = 0;
	ar->channel_data = 0;
	return ar;
}
void AudioManager::unloadSampleAudioResource(AudioResourceInfo* ar_info){
	
}