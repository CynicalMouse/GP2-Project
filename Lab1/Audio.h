#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <AL/efx-creative.h>
#include <AL/EFX-Util.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <vector>

class Audio
{
public:
	// CONSTRUCTOR & DECONSTRUCTOR //
	Audio();
	~Audio();

	enum EffectType
	{
		NONE,
		ECHO,
		DISTORTION,
		REVERB,
		PITCHSHIFT,
	};

	// AUDIO PLAYBACK //
	void playAudio(unsigned int id, EffectType effectType);
	void playAudioDistance(unsigned int id, EffectType effectType, float listenerDistance);
	void stopAudio(unsigned int id);

	// AUDIO MANAGEMENT //
	void changeVolume(unsigned int id, float volume);

	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);
	unsigned int loadSound(const char* filename);

private:
	// UTILITY //	
	bool checkForErrors(int line);
	int convertToInt(char* buffer, int length);
	bool checkBigEndian();
	void loadFunctionPointers();


	// EFFECTS //
	void playEcho(unsigned int id);
	void playDistortion(unsigned int id);
	void playReverb(unsigned int id);
	void playPitchShift(unsigned int id);
	
#pragma region EFX Objects
	// Effect objects
	LPALGENEFFECTS alGenEffects;
	LPALDELETEEFFECTS alDeleteEffects;
	LPALISEFFECT alIsEffect;
	LPALEFFECTI alEffecti;
	LPALEFFECTIV alEffectiv;
	LPALEFFECTF alEffectf;
	LPALEFFECTFV alEffectfv;
	LPALGETEFFECTI alGetEffecti;
	LPALGETEFFECTIV alGetEffectiv;
	LPALGETEFFECTF alGetEffectf;
	LPALGETEFFECTFV alGetEffectfv;

	//Filter objects
	LPALGENFILTERS alGenFilters;
	LPALDELETEFILTERS alDeleteFilters;
	LPALISFILTER alIsFilter;
	LPALFILTERI alFilteri;
	LPALFILTERIV alFilteriv;
	LPALFILTERF alFilterf;
	LPALFILTERFV alFilterfv;
	LPALGETFILTERI alGetFilteri;
	LPALGETFILTERIV alGetFilteriv;
	LPALGETFILTERF alGetFilterf;
	LPALGETFILTERFV alGetFilterfv;

	// Auxiliary slot object
	LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
	LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
	LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
	LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
	LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
	LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
	LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
	LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
	LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
	LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
	LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

#pragma endregion

	ALuint effectSlot;
	ALuint effect;



	ALCdevice* device;
	ALCcontext* context;


	// AUDIO DATA STORAGE //
	struct data {
		int sourceID, bufferID;
		char* buffer;
		std::string name;
		data(unsigned int sI, unsigned int bI, char* b, const char* n)
		{
			this->sourceID = sI;
			this->bufferID = bI;
			this->buffer = b;
			this->name = n;
		}

	};

	std::vector<data> datas;
};

