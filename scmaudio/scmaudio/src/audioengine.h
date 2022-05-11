#pragma once

#include <RtAudio.h>

#include "audiodevice.h"
#include "scmaudiocallback.h"
#include "result.h"
#include "soundmanager.h"
#include "soundinstance.h"
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
    Result<> SetDefaultDevices();

    AudioEngine& SetBufferSize(U32);
    AudioEngine& SetMaxPolyphony(U32);
    AudioEngine& SetSamplingRate(U32);

    Result<> Ignite(); // "Initialize" is so boring...
    void Shutdown();

    Result<SoundId> LoadSound(const String& path);
    Result<> PlaySound(SoundId soundId);

    Status GetStatus() { return _status; }
    bool StatusIs(Status status) { return _status == status; }
    bool StatusIsNot(Status status) { return _status != status; }

    ErrorSource GetErrorSource() { return _errorSource; }
    const AudioDevice& GetInputDevice() { return _inputDevice; }
    const AudioDevice& GetOutputDevice() { return _outputDevice; }


private:
    void ErrorCallback(RtAudioErrorType error, const String& errorMessage);
    Result<> InitializeRtAudio();
    Result<> StopRtAudio();

    static void SetDeviceParameters(const AudioDevice& device, AudioDevice::Flow flow, RtAudio::StreamParameters& parameters);
    Result<> SelectSamplingRate();

private:
    U32 _bufferSize;
    U32 _maxPolyphony;
    U32 _samplingRate;
    Status _status;
    AudioDevice _inputDevice;
    AudioDevice _outputDevice;
    ErrorSource _errorSource;
    RtAudioErrorType _rtAudioError;
#pragma warning(push)
#pragma warning(disable:4251)
    SoundStore _soundStore;
    UniquePtr<RtAudio> _rtAudio;
    String _errorMessage;
    ObjectPool<SoundInstance> _soundInstancePool;
    List<SharedPtr<SoundInstance>> _processedSoundInstance;
#pragma warning(pop)
};

} // namespace ScmAudio