//#pragma once
//
//#ifndef H_AUDIOMANAGER
//#define H_AUDIOMANAGER
//
//#include <string>
//#include <Windows.h>
//#include "fmod.h"
//#include "fmod.hpp"
//#include "fmod_codec.h"
//#include "fmod_dsp.h"
//#include "fmod_errors.h"
//#include "fmod_memoryinfo.h"
//#include "fmod_output.h"
//
//class AudioManager
//{
//public:
//	// Destructor
//	~AudioManager(void);
//
//	void Initialize(void);  // Initialize sound components
//	void Shutdown(void);    // Shutdown sound components
//
//	// Singleton instance manip methods
//	static AudioManager* GetInstance(void);
//	static void DestroyInstance(void);
//
//	// Accessors
//	FMOD::System* GetSystem(void)
//	{
//		return soundSystem;
//	}
//
//	// Sound playing
//	void Play(FMOD::Sound* sound);  // Play a sound/music with default channel
//	void PlaySFX(FMOD::Sound* sound);   // Play a sound effect with custom channel
//	void PlayBGM(FMOD::Sound* sound);   // Play background music with custom channel
//
//	// Volume adjustment methods
//	void SetBGMVolume(float volume);
//	void SetSFXVolume(float volume);
//
//private:
//	static AudioManager* instance;  // Singleton instance
//	AudioManager(void);  // Constructor
//
//	FMOD::System* soundSystem;  // Sound system object
//	FMOD_RESULT result;
//	FMOD::Channel* bgmChannel;  // Channel for background music
//	static const int numSfxChannels = 4;
//	FMOD::Channel* sfxChannels[numSfxChannels]; // Channel for sound effects
//};
//
//#endif
