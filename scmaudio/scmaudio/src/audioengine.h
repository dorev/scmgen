#pragma once

#include <RtAudio.h>

#include "audiodevice.h"
#include "scmaudiocallback.h"
#include "result.h"
#include "soundmanager.h"
#include "types.h"

namespace ScmAudio
{

struct RtAudioHelper
{
    static Error ErrorConverter(RtAudioErrorType error)
    {
        switch (error)
        {
        case RTAUDIO_WARNING: return MAKE_ERROR(WARNING);
        case RTAUDIO_UNKNOWN_ERROR: return MAKE_ERROR(UNKNOWN_ERROR);
        case RTAUDIO_NO_DEVICES_FOUND: return MAKE_ERROR(NO_DEVICES_FOUND);
        case RTAUDIO_INVALID_DEVICE: return MAKE_ERROR(INVALID_DEVICE);
        case RTAUDIO_DEVICE_DISCONNECT: return MAKE_ERROR(DEVICE_DISCONNECT);
        case RTAUDIO_MEMORY_ERROR: return MAKE_ERROR(MEMORY_ERROR);
        case RTAUDIO_INVALID_PARAMETER: return MAKE_ERROR(INVALID_PARAMETER);
        case RTAUDIO_INVALID_USE: return MAKE_ERROR(INVALID_USE);
        case RTAUDIO_DRIVER_ERROR: return MAKE_ERROR(DRIVER_ERROR);
        case RTAUDIO_SYSTEM_ERROR: return MAKE_ERROR(SYSTEM_ERROR);
        case RTAUDIO_THREAD_ERROR: return MAKE_ERROR(THREAD_ERROR);
        default:
            return MAKE_ERROR(Undefined);
        }
    }
};

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
#pragma warning(pop)
};

} // namespace ScmAudio