#if !defined AUDIO_PLAYER
#define AUDIO_PLAYER

class AudioResource;

class AudioPlayer {
	private:
		AudioResource* audio_resource;

		int num_repeats;  //0 play once, 1 play twice, etc.
		int repeat_count;

	public:
	AudioPlayer(AudioResource* audio_resource, int num_plays);
		virtual ~AudioPlayer();

		int getNumRepeats();
		int getRepeatCount();
		void incRepeatCount();
		AudioResource* getAudioResource();
		bool isAudioPlayerDone();

};
#endif
