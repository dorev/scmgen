#pragma once

#include "audioengine.h"
#include "utils.h"

namespace ScmAudio
{

// http://web.mit.edu/carrien/Public/speechlab/marc_code/ADAPT_VC/rtaudio/doc/html/index.html

AudioEngine::AudioEngine()
    : _bufferSize(1024)
    , _maxPolyphony(32)
    , _samplingRate(44100)
    , _status(Stopped)
    , _rtAudio(nullptr)
    , _rtAudioError(RtAudioErrorType::RTAUDIO_NO_ERROR)
    , _errorSource(ErrorSource::None)
    , _errorMessage("")
{
    InitializeRtAudio();
}

AudioEngine::~AudioEngine()
{
}

bool AudioEngine::InitializeRtAudio()
{
    if (_rtAudio == nullptr)
    {
        using namespace std::placeholders;
        _rtAudio = MakeUnique<RtAudio>(
            RtAudio::Api::UNSPECIFIED,
            std::bind(&AudioEngine::ErrorCallback, this, _1, _2)
        );
    }
    else if(StatusIsNot(InError) && StatusIs(Running))
    {
        // useless call, RtAudio is fine and running
    }
    else
    {
        // something is wrong, reset RtAudio
    }
    return StatusIs(Running);
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
    }

    return Success;
}

AudioEngine& AudioEngine::SetBufferSize(U32 bufferSize)
{
    _bufferSize = bufferSize;
    return *this;
}

AudioEngine& AudioEngine::SetMaxPolyphony(U32 maxPolyphony)
{
    _maxPolyphony = maxPolyphony;
    return *this;
}

AudioEngine& AudioEngine::SetSamplingRate(U32 samplingRate)
{
    _samplingRate = samplingRate;
    return *this;
}

Result<> AudioEngine::Ignite()
{
    if (!InitializeRtAudio())
        return MAKE_ERROR(RtAudioNotInitialized);

    if (!_inputDevice.IsValid() && !_outputDevice.IsValid())
        return MAKE_ERROR(NoValidDeviceAvailable);

    Result<> samplingRateSelectionResult = SelectSamplingRate();
    RETURN_ON_ERROR(samplingRateSelectionResult);


    RtAudio::StreamParameters inputParameters;
    RtAudio::StreamParameters outputParameters;
    SetDeviceParameters(_outputDevice, AudioDevice::Flow::Output, outputParameters);
    SetDeviceParameters(_inputDevice, AudioDevice::Flow::Input, inputParameters);

    U32 bufferSizeTmp = _bufferSize;
    RtAudioErrorType openStreamResult = _rtAudio->openStream(
        &outputParameters,
        &inputParameters,
        RTAUDIO_FLOAT32,
        _samplingRate,
        &bufferSizeTmp,
        &ScmAudioCallback,
        ToVoidPtr(this)
    );

    if (openStreamResult != RTAUDIO_NO_ERROR)
        return RtAudioHelper::ErrorConverter(openStreamResult);

    if (bufferSizeTmp != _bufferSize)
        return MAKE_ERROR(UnsupportedBufferSize);

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

} // namespace ScmAudio