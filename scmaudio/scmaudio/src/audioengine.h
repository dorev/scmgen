#pragma once

#include "audiodevice.h"
#include "types.h"
#include "result.h"
#include "sound.h"
#include <RtAudio.h>

namespace ScmAudio
{

class AudioEngine
{
public:
    
    enum class Status
    {
        Uninitialized,
        Idle,
        Running,
        ShuttingDown
    };

    AudioEngine();

    Vector<AudioDevice> ListAudioDevices();

    AudioEngine& SetInputDevice(const AudioDevice&);
    AudioEngine& SetOutputDevice(const AudioDevice&);
    AudioEngine& SetMaxPolyphony(U32);
    Result<> Ignite(); // "Initialize" is so boring...
    
    struct SessionReport;
    SessionReport Shutdown();

    Sound& Load(const String& path);
    Result<> Unload(SoundId);

    Result<> Unload(Sound&);
    Sound& GetSound(SoundId);
    Sound& GetSound(const String&);

    SoundInstance PlayLoaded(const Sound&);
    SoundInstance PlayLoaded(SoundId);

    AudioEngine& StopAll();
    AudioEngine& StopAll(SoundId);
    AudioEngine& StopAll(const Sound&);
    AudioEngine& Stop(const SoundInstance&);

private:
    // http://web.mit.edu/carrien/Public/speechlab/marc_code/ADAPT_VC/rtaudio/doc/html/index.html
    void AudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData);

private:
    U32 _maxPolyphony;
    Status _status;
    RtAudio* _rtAudio;
    AudioDevice _inputDevice;
    AudioDevice _outputDevice;
};

} // namespace ScmAudio