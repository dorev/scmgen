#pragma once

#include <RtAudio.h>

#include "audiodevice.h"
#include "result.h"
#include "sound.h"
#include "types.h"

namespace ScmAudio
{

class SCMAUDIO_API AudioEngine
{
public:
    enum class ErrorSource
    {
        None,
        ScmAudio,
        RtAudio
    };

    // enum chosen over enum class for readability in code
    enum Status
    {
        InError,
        Stopped,
        Running
    };

public:
    AudioEngine();
    ~AudioEngine();

    Result<const Vector<AudioDevice>> ListAudioDevices();

    Result<> SetInputDevice(const AudioDevice&);
    Result<> SetOutputDevice(const AudioDevice&);
    Result<> SetMaxPolyphony(U32);

    Result<> Ignite(); // "Initialize" is so boring...
    void Shutdown();

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

    Status GetStatus() { return _status; }
    bool StatusIs(Status status) { return _status == status; }
    bool StatusIsNot(Status status) { return _status != status; }

    ErrorSource GetErrorSource() { return _errorSource; }

private:
    // http://web.mit.edu/carrien/Public/speechlab/marc_code/ADAPT_VC/rtaudio/doc/html/index.html
    void AudioCallback(void* outputBuffer, void* inputBuffer, U32 nFrames, F64 streamTime, RtAudioStreamStatus status, void* userData);
    void ErrorCallback(RtAudioErrorType error, const String& errorMessage);
    bool InitializeRtAudio();
    Result<> StopRtAudio();

private:
    U32 _maxPolyphony;
    Status _status;
    AudioDevice _inputDevice;
    AudioDevice _outputDevice;
    ErrorSource _errorSource;
    RtAudioErrorType _rtAudioError;

#pragma warning(push)
#pragma warning(disable:4251)
    UniquePtr<RtAudio> _rtAudio;
    String _errorMessage;
#pragma warning(pop)
};

} // namespace ScmAudio