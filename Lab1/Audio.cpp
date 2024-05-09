#include "Audio.h"

// CONSTRUCTOR & DECONSTRUCTOR //
#pragma region Contructor & Deconstructor

Audio::Audio()
{
	// Open sound card
	device = alcOpenDevice(NULL);

	// If sound card is valid, create context
	context = (device != NULL) ? alcCreateContext(device, NULL) : (std::cout << "Can't open sound card: " << device << std::endl, NULL);

	// If context is valid, set it to current context
	context = (context != NULL) ? (alcMakeContextCurrent(context), context) : (std::cout << "Can't open context: " << context << std::endl, NULL);

	// Check for EFX extension
	if (alcIsExtensionPresent(device, "ALC_EXT_EFX") == AL_FALSE)
	{
		checkForErrors(__LINE__);
	}

	loadFunctionPointers();
}

Audio::~Audio()
{
	
}

#pragma endregion

// UTILITY //
#pragma region Utility

// Checks for openAL errors and details which type
bool Audio::checkForErrors(int line)
{
	ALenum error = alGetError();

	if (error != AL_NO_ERROR)
	{
		std::cerr << "//==// OPENAL ERROR //==//" << " At line :" << line << std::endl;

		switch (error)
		{
		case AL_INVALID_NAME:
			std::cerr << "AL_INVALID_NAME: An invalid ID was passed." << std::endl;
			break;

		case AL_INVALID_ENUM:
			std::cerr << "AL_INVALID_ENUM: An invalid enum value was passed." << std::endl;
			break;

		case AL_INVALID_VALUE:
			std::cerr << "AL_INVALID_VALUE: An invalid value was passed." << std::endl;
			break;

		case AL_INVALID_OPERATION:
			std::cerr << "AL_INVALID_OPERATION: Requested operation is invalid." << std::endl;
			break;

		case AL_OUT_OF_MEMORY:
			std::cerr << "AL_OUT_OF_MEMORY: OpenAL ran out of memory due to requested operation." << std::endl;
			break;

		default:
			std::cerr << "UNKOWN OPENAL ERROR: " << error << std::endl;
		}
		return false;
	}
	return true;
}

// convert bytes to integer
int Audio::convertToInt(char* buffer, int length)
{
	int a = 0;
	if (!checkBigEndian())
	{
		// Big endian, store bytes in large to small order
		std::memcpy(&a, buffer, length);
	}
	else
	{
		for (int i = 0; i < length; i++)
		{
			// Little endian, store in reverse order
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
		}
	}
	return a;
}

// Check if bytes are stored largest to smallest (Big endian)
bool Audio::checkBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

void Audio::loadFunctionPointers()
{
	alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
	alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");

	alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");
	alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
	alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
	alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
	alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");

	alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
	alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
	alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
	alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");

	alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
	alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
	alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
	alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
	alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
	alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
	alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");

	alGetFilteri = (LPALGETFILTERI)alGetProcAddress("alGetFilteri");
	alGetFilteriv = (LPALGETFILTERIV)alGetProcAddress("alGetFilteriv");
	alGetFilterf = (LPALGETFILTERF)alGetProcAddress("alGetFilterf");
	alGetFilterfv = (LPALGETFILTERFV)alGetProcAddress("alGetFilterfv");

	alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
	alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");

	alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
	alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
	alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
	alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
	alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");

	alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
	alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
	alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
	alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");
}

#pragma endregion

// AUDIO PLAYBACK //
#pragma region Audio Playback

void Audio::playAudio(unsigned int id, EffectType effectType)
{
	switch (effectType) 
	{
		case NONE:
			// Remove any effects applied to sound
			alSource3i(id, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
			alSourcePlay(id);
			break;

		case ECHO:
			playEcho(id);
			break;

		case DISTORTION:
			playDistortion(id);
			break;

		case REVERB:
			playReverb(id);
			break;

		case PITCHSHIFT:
			playPitchShift(id);
			break;

		default:
			std::cerr << "Unknown effect type: " << effectType << std::endl;
			return;
	}
}

void Audio::playAudioDistance(unsigned int id, EffectType effectType, float listenerDistance)
{
	switch (effectType)
	{
	case NONE:
	{
		// Remove any effects applied to sound
		alSource3i(id, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);

		// Calculate attenuation factor based on distance from listener
		float attenuation_factor = 1.0f / listenerDistance * 3.5f;
		alSourcef(id, AL_GAIN, attenuation_factor);

		alSourcePlay(id);
		break;
	}
		
	case ECHO:
		playEcho(id);
		break;

	case DISTORTION:
		playDistortion(id);
		break;

	case REVERB:
		playReverb(id);
		break;

	case PITCHSHIFT:
		playPitchShift(id);
		break;

	default:
		std::cerr << "Unknown effect type: " << effectType << std::endl;
		return;
	}
}

void Audio::stopAudio(unsigned int id)
{
	alSourceStop(id);
	checkForErrors(__LINE__);
}

// Effects
// FOR SOME REASON ONLY REVERB WORKS
// ALL OTHERS RETURN AN INVALID ENUM WHEN TRYING TO SET EFFECT TYPE, 
// EVEN WHEN TRIED IN SAMPLE CODE FROM OPEN AL(OPENAL SDK EFXReverb2005)
void Audio::playEcho(unsigned int id)
{
	// Generate effect
	alGenEffects(1, &effect);
	checkForErrors(__LINE__);


	// Set effect type
	alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_ECHO);
	checkForErrors(__LINE__);


	// Set effect parameters
	alEffectf(effect, AL_ECHO_DELAY, 0.05f); // Maximum delay
	alEffectf(effect, AL_ECHO_LRDELAY, 0.1f); // Maximum LR delay
	alEffectf(effect, AL_ECHO_DAMPING, 0.0f); // No damping
	alEffectf(effect, AL_ECHO_FEEDBACK, 1.0f); // High feedback
	alEffectf(effect, AL_ECHO_SPREAD, -1.0f); // Maximum spread
	checkForErrors(__LINE__);

	// Generate effect slot
	alGenAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);


	// Attach effect to effect slot
	alAuxiliaryEffectSloti(effectSlot, AL_EFFECTSLOT_EFFECT, effect);
	checkForErrors(__LINE__);

	// Apply effect
	alSource3i(id, AL_AUXILIARY_SEND_FILTER, effectSlot, 0, AL_FILTER_NULL);
	checkForErrors(__LINE__);

	// Play sound
	alSourcePlay(id);
	checkForErrors(__LINE__);

	// Delete effect
	alDeleteEffects(1, &effect);
	alDeleteAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);
}

void Audio::playDistortion(unsigned int id)
{
	// Generate effect
	alGenEffects(1, &effect);
	checkForErrors(__LINE__);


	// Set effect type
	alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_DISTORTION);
	checkForErrors(__LINE__);

	// Set effect parameters
	alEffectf(effect, AL_DISTORTION_EDGE, 0.5f);
	alEffectf(effect, AL_DISTORTION_GAIN, 0.5f);
	alEffectf(effect, AL_DISTORTION_LOWPASS_CUTOFF, 8000);
	alEffectf(effect, AL_DISTORTION_EQCENTER, 3600);
	alEffectf(effect, AL_DISTORTION_EQBANDWIDTH, 3600);
	checkForErrors(__LINE__);

	// Generate effect slot
	alGenAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);

	// Attach effect to effect slot
	alAuxiliaryEffectSloti(effectSlot, AL_EFFECTSLOT_EFFECT, effect);
	checkForErrors(__LINE__);

	// Apply effect
	alSource3i(id, AL_AUXILIARY_SEND_FILTER, effectSlot, 0, AL_FILTER_NULL);
	checkForErrors(__LINE__);

	// Play sound
	alSourcePlay(id);
	checkForErrors(__LINE__);

	// Delete effect
	alDeleteEffects(1, &effect);
	alDeleteAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);
}

void Audio::playReverb(unsigned int id)
{
	// Generate effect
	alGenEffects(1, &effect);
	checkForErrors(__LINE__);


	// Set effect type
	alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);
	checkForErrors(__LINE__);

	// Set effect parameters
	alEffectf(effect, AL_REVERB_DENSITY, 0.5f);
	alEffectf(effect, AL_REVERB_DIFFUSION, 1.0f);
	alEffectf(effect, AL_REVERB_GAIN, 0.75f);
	alEffectf(effect, AL_REVERB_DECAY_TIME, 3.0f);
	alEffectf(effect, AL_REVERB_DECAY_HFRATIO, 0.7f);
	alEffectf(effect, AL_REVERB_REFLECTIONS_GAIN, 0.05f);
	alEffectf(effect, AL_REVERB_LATE_REVERB_GAIN, 1.26f);
	checkForErrors(__LINE__);

	// Generate effect slot
	alGenAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);

	// Attach effect to effect slot
	alAuxiliaryEffectSloti(effectSlot, AL_EFFECTSLOT_EFFECT, effect);
	checkForErrors(__LINE__);

	// Apply effect to sound
	alSource3i(id, AL_AUXILIARY_SEND_FILTER, effectSlot, 0, AL_FILTER_NULL);
	checkForErrors(__LINE__);

	// Play sound
	alSourcePlay(id);
	checkForErrors(__LINE__);

	// Delete effect
	alDeleteEffects(1, &effect);
	alDeleteAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);
}

void Audio::playPitchShift(unsigned int id)
{
	// Generate effect
	alGenEffects(1, &effect);
	checkForErrors(__LINE__);

	// Set effect type
	alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_PITCH_SHIFTER);
	checkForErrors(__LINE__);

	// Set parameters
	alEffecti(effect, AL_PITCH_SHIFTER_COARSE_TUNE, 12); // Up one octave
	alEffecti(effect, AL_PITCH_SHIFTER_FINE_TUNE, 0); // No fine tuning
	checkForErrors(__LINE__);

	// Create effect slot
	alGenAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);

	// Attach effect to effect slot
	alAuxiliaryEffectSloti(effectSlot, AL_EFFECTSLOT_EFFECT, effect);
	checkForErrors(__LINE__);

	// Apply effect
	alSource3i(id, AL_AUXILIARY_SEND_FILTER, effectSlot, 0, AL_FILTER_NULL);
	checkForErrors(__LINE__);

	// Play sound
	alSourcePlay(id);
	checkForErrors(__LINE__);

	// Delete effect
	alDeleteEffects(1, &effect);
	alDeleteAuxiliaryEffectSlots(1, &effectSlot);
	checkForErrors(__LINE__);
}

#pragma endregion

// AUDIO MANAGEMENT //
#pragma region Audio Management

void Audio::changeVolume(unsigned int id, float volume)
{
	// Clamp volume to 0.0 - 1.0
	volume = std::max(0.0f, std::min(1.0f, volume));

	// Set volume
	alSourcef(id, AL_GAIN, volume);
	checkForErrors(__LINE__);
}

#pragma endregion

// LOADING SOUND //
// GOT CHATGPT/WINDOWS COPILOT HELP TO READ WAV FILE
char* Audio::loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
    char buffer[4];
    std::ifstream in(fn, std::ios::binary);
    if (!in) {
        std::cerr << "Could not open file: " << fn << std::endl;
        return nullptr;
    }

    // Read RIFF header
    in.read(buffer, 4);
    if (strncmp(buffer, "RIFF", 4) != 0) {
        std::cerr << "Not a valid WAV file: " << fn << std::endl;
        return nullptr;
    }

    // Skip file size and file type
    in.seekg(8, std::ios::cur);

    // Read format chunk
    in.read(buffer, 4);
    if (strncmp(buffer, "fmt ", 4) != 0) {
        std::cerr << "Not a valid WAV file: " << fn << std::endl;
        return nullptr;
    }

    // Skip chunk size
    in.seekg(4, std::ios::cur);

    // Read format type
    in.read(buffer, 2);
    short formatType = convertToInt(buffer, 2);
    if (formatType != 1) { // 1 = PCM
        std::cerr << "Unsupported WAV format: " << fn << std::endl;
        return nullptr;
    }

    // Read channels
    in.read(buffer, 2);
    chan = convertToInt(buffer, 2);

    // Read sample rate
    in.read(buffer, 4);
    samplerate = convertToInt(buffer, 4);

    // Skip byte rate and block align
    in.seekg(6, std::ios::cur);

    // Read bits per sample
    in.read(buffer, 2);
    bps = convertToInt(buffer, 2);

    // Read data chunk header
    in.read(buffer, 4);
    if (strncmp(buffer, "data", 4) != 0) {
        std::cerr << "Not a valid WAV file: " << fn << std::endl;
        return nullptr;
    }

    // Read data size
    in.read(buffer, 4);
    size = convertToInt(buffer, 4);

    // Read data
    char* soundData = new char[size];
    in.read(soundData, size);
    return soundData;
}

unsigned int Audio::loadSound(const char* filename)
{
	bool found = false;
	unsigned int sourceID, bufferID;
	char* soundData = NULL;

	for (int i = 0; i < datas.size(); i++)
	{
		if (datas[i].name == filename && datas[i].bufferID != -1)
		{
			bufferID = datas[i].bufferID;
			found = true;
			break;
		}
	}

	if(!found)
		{	
			int channel, sampleRate, bps, size;
			soundData = loadWAV(filename, channel, sampleRate, bps, size);
			unsigned int format;
			alGenBuffers(1, &bufferID);

            if (channel == 1)
            {
                format = (bps == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
            }
            else
            {
                format = (bps == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
            }

			alBufferData(bufferID, format, soundData, size, sampleRate);
		}

	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, 1.0f);
	datas.push_back(data(sourceID, (!found ? bufferID : -1), soundData, filename));
	return sourceID;
}