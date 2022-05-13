#pragma once

#include <RtAudio.h>

#include "audiodevice.h"
#include "soundplayer.h"
#include "scmaudiocallback.h"
#include "result.h"
#include "soundstore.h"
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

    Result<> SetInputDevice(const AudioDevice& inputDevice);
    Result<> SetOutputDevice(const AudioDevice& outputDevice);
    Result<> SetDefaultDevices();

    // Builder methods
    AudioEngine& SetBufferSize(U32 bufferSize) { _bufferSize = bufferSize; return *this; }
    AudioEngine& SetSamplingRate(U32 samplingRate) { _samplingRate = samplingRate; return *this; }
    AudioEngine& SetMaxPolyphony(U32 maxPolyphony) { _maxPolyphony = maxPolyphony; return *this; }
    AudioEngine& SetMute(bool mute = true) { _mute = mute; return *this; }
    AudioEngine& SetCapture(bool capture = true) { _capture = capture; return *this; }

    Result<> Ignite(); // "Initialize" is so boring...
    void Shutdown();

    Result<SoundId> LoadSound(const String& path);
    Result<SoundInstancePtr> Play(SoundId soundId);
    Result<SoundInstancePtr> Play(const String& soundPath);

    Status GetStatus() { return _status; }
    bool StatusIs(Status status) { return _status == status; }
    bool StatusIsNot(Status status) { return _status != status; }

    ErrorSource GetErrorSource() { return _errorSource; }
    const AudioDevice& GetInputDevice() { return _inputDevice; }
    const AudioDevice& GetOutputDevice() { return _outputDevice; }

    bool IsMuted() const { return _outputDevice.outputChannels == 0 || _mute; }
    bool IsCapturing() const { return _inputDevice.inputChannels > 0 && _capture; }

private:
    friend int AudioCallback(void*, void*, U32, F64, RtAudioStreamStatus, void*);
    SoundPlayer& GetPlayer() { return _soundPlayer; }

    void ErrorCallback(RtAudioErrorType error, const String& errorMessage);
    Result<> InitializeRtAudio();
    Result<> StopRtAudio();

    static void SetDeviceParameters(const AudioDevice& device, AudioDevice::Flow flow, RtAudio::StreamParameters& parameters);
    Result<> SelectSamplingRate();

private:
    U32 _bufferSize;
    U32 _samplingRate;
    U32 _maxPolyphony;
    Status _status;
    AudioDevice _inputDevice;
    AudioDevice _outputDevice;
    bool _mute;
    bool _capture;
    ErrorSource _errorSource;
    RtAudioErrorType _rtAudioError;
#pragma warning(push)
#pragma warning(disable:4251)
    SoundStore _soundStore;
    UniquePtr<RtAudio> _rtAudio;
    String _errorMessage;
    SoundPlayer _soundPlayer;
#pragma warning(pop)
};

} // namespace ScmAudio