#include "audioengine.h"
#include "rtaudiohelper.h"
#include "utils.h"

namespace ScmAudio
{

AudioEngine::AudioEngine()
    : _bufferSize(1024)
    , _maxPolyphony(32)
    , _samplingRate(44100)
    , _status(Stopped)
    , _rtAudio(nullptr)
    , _rtAudioError(RtAudioErrorType::RTAUDIO_NO_ERROR)
    , _outputMixDoneCallback(nullptr)
    , _errorSource(ErrorSource::None)
    , _errorMessage("")
    , _soundPlayer(_samplingRate, _maxPolyphony)
    , _mute(false)
    , _capture(true)
{
    StopRtAudio();
    InitializeRtAudio();
}

AudioEngine::~AudioEngine()
{
    Stop();
}

void AudioEngine::Stop()
{
    if(StatusIs(Running))
        StopRtAudio();
}

Result<> AudioEngine::InitializeRtAudio()
{
    if (_rtAudio == nullptr)
    {
        using namespace std::placeholders;
        _rtAudio = MakeUnique<RtAudio>(
            RtAudio::Api::UNSPECIFIED,
            std::bind(&AudioEngine::ErrorCallback, this, _1, _2)
        );
    }

    if(StatusIs(InError))
    {
        if (_errorSource == ErrorSource::RtAudio)
            return RtAudioHelper::ErrorConverter(_rtAudioError);

        return MAKE_ERROR(RtAudioNotInitialized);
    }

    return Success;
}

Result<> AudioEngine::StopRtAudio()
{
    if (_rtAudio == nullptr)
        return MAKE_ERROR(RtAudioNotInitialized);

    if (StatusIs(InError))
        return MAKE_ERROR(AudioEngineInError);

    _rtAudio->stopStream();
    if(StatusIs(InError))
        return MAKE_ERROR(AudioEngineInError);

    _rtAudio->closeStream();
    if (StatusIs(InError))
        return MAKE_ERROR(AudioEngineInError);

    return Success;
}

void AudioEngine::ErrorCallback(RtAudioErrorType error, const String& errorMessage)
{
    _status = Status::InError;
    _errorSource = ErrorSource::RtAudio;
    _rtAudioError = error;
    _errorMessage = errorMessage;
}

Result<const Vector<AudioDevice>> AudioEngine::ListAudioDevices()
{
    if(StatusIs(InError))
        return MAKE_ERROR(AudioEngineInError);

    Vector<AudioDevice> deviceList;
    RtAudio::DeviceInfo rtDevice;
    U32 deviceCount = _rtAudio->getDeviceCount();
    for (U32 i = 0; i < deviceCount; i++)
    {
        rtDevice = _rtAudio->getDeviceInfo(i);
        if (StatusIsNot(InError) && rtDevice.probed)
            deviceList.emplace_back(rtDevice, i);
        else
            return MAKE_ERROR(UnableToProbeDevice);
    }

    return deviceList;
}

Result<> AudioEngine::SetInputDevice(const AudioDevice& device)
{
    if (device.supported == false)
        return MAKE_ERROR(DeviceNotSupported);

    if (device != _inputDevice)
    {
        if (StatusIsNot(Stopped))
            return MAKE_ERROR(EngineAlreadyRunning);

        _inputDevice = device;
    }

    return Success;
}

Result<> AudioEngine::SetOutputDevice(const AudioDevice& device)
{
    if (device.supported == false)
        return MAKE_ERROR(DeviceNotSupported);

    if (device != _outputDevice)
    {
        if (StatusIsNot(Stopped))
            return MAKE_ERROR(EngineAlreadyRunning);

        _outputDevice = device;
        _soundPlayer.SetChannels(_outputDevice.outputChannels);
    }

    return Success;
}

Result<> AudioEngine::SetDefaultDevices()
{
    if (_rtAudio == nullptr)
        return MAKE_ERROR(RtAudioNotInitialized);

    Result<const Vector<AudioDevice>> listDevicesResult = ListAudioDevices();
    RETURN_ON_ERROR(listDevicesResult);

    const Vector<AudioDevice>& deviceList = listDevicesResult.GetValue();
    _inputDevice = deviceList[_rtAudio->getDefaultInputDevice()];
    _outputDevice = deviceList[_rtAudio->getDefaultOutputDevice()];

    return Success;
}

Result<> AudioEngine::Ignite()
{
    Result<> initializeResult = InitializeRtAudio();
    if (initializeResult.HasError())
        return initializeResult;

    if (!_inputDevice.IsValid() && !_outputDevice.IsValid())
        return MAKE_ERROR(NoValidDeviceAvailable);

    Result<> samplingRateSelectionResult = SelectSamplingRate();
    RETURN_ON_ERROR(samplingRateSelectionResult);


    RtAudio::StreamParameters inputParameters;
    RtAudio::StreamParameters outputParameters;
    SetDeviceParameters(_outputDevice, AudioDevice::Flow::Output, outputParameters);
    SetDeviceParameters(_inputDevice, AudioDevice::Flow::Input, inputParameters);

    RtAudio::StreamOptions streamOptions;
    streamOptions.flags = RTAUDIO_MINIMIZE_LATENCY;

    U32 bufferSizeTmp = _bufferSize;
    RtAudioErrorType openStreamResult = _rtAudio->openStream(
        &outputParameters,
        &inputParameters,
        RTAUDIO_FLOAT32,
        _samplingRate,
        &bufferSizeTmp,
        &AudioCallback,
        ToPtr<void>(this),
        &streamOptions
    );

    if (openStreamResult != RTAUDIO_NO_ERROR)
        return RtAudioHelper::ErrorConverter(openStreamResult);

    if (bufferSizeTmp != _bufferSize)
        return MAKE_ERROR(UnsupportedBufferSize);

    RtAudioErrorType startResult = _rtAudio->startStream();
    if (startResult != RTAUDIO_NO_ERROR)
        return RtAudioHelper::ErrorConverter(startResult);

    return Success;
}

Result<> AudioEngine::SelectSamplingRate()
{
    if (!_inputDevice.IsValid() && !_outputDevice.IsValid())
        return MAKE_ERROR(NoValidDeviceAvailable);

    if (_inputDevice.IsValid() && !Contains(_inputDevice.supportedSampleRates, _samplingRate))
        return MAKE_ERROR(UnsupportedInputSamplingRate);

    if (_outputDevice.IsValid() && !Contains(_outputDevice.supportedSampleRates, _samplingRate))
        return MAKE_ERROR(UnsupportedOutputSamplingRate);

    return Success;
}

void AudioEngine::SetDeviceParameters(const AudioDevice& device, AudioDevice::Flow flow, RtAudio::StreamParameters& parameters)
{
    if (device.id == InvalidId)
    {
        // Reset parameters if device is invalid
        parameters = RtAudio::StreamParameters();
        return;
    }

    parameters.deviceId = device.id;
    parameters.nChannels = (flow == AudioDevice::Flow::Input) ? device.inputChannels : device.outputChannels;
}


Result<SoundId> AudioEngine::LoadSound(const String& path)
{
    return _soundStore.Load(path);
}

Result<SoundInstancePtr> AudioEngine::Play(SoundId soundId)
{
    const Sound& sound = _soundStore.GetSound(soundId);
    if (!sound.IsValid())
        return MAKE_ERROR(SoundNotAvailable);

    return _soundPlayer.AddSoundInstance(sound);
}

Result<SoundInstancePtr> AudioEngine::Play(const String& soundPath)
{
    const Sound& sound = _soundStore.GetSound(soundPath);
    if (!sound.IsValid())
        return MAKE_ERROR(SoundNotAvailable);

    return _soundPlayer.AddSoundInstance(sound);
}

void AudioEngine::OnOutputMixDone(void* outputBuffer, U32 bufferSize, U32 channels)
{
    if (_outputMixDoneCallback != nullptr)
        _outputMixDoneCallback(outputBuffer, bufferSize, channels);
}

} // namespace ScmAudio
