#pragma once

#include "audioengine.h"

namespace ScmAudio
{

// http://web.mit.edu/carrien/Public/speechlab/marc_code/ADAPT_VC/rtaudio/doc/html/index.html

AudioEngine::AudioEngine()
    : _maxPolyphony(32)
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
    if (_rtAudio == nullptr || StatusIs(Stopped))
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
        {
            deviceList.emplace_back(rtDevice, i);
        }
        else
        {
            return MAKE_ERROR(UnableToProbeDevice);
        }
    }

    return deviceList;
}

Result<> AudioEngine::SetInputDevice(const AudioDevice& device)
{
    if (device.supported == false)
        return MAKE_ERROR(DeviceNotSupported);

    if (device != _inputDevice && StatusIs(Running))
    {
        if (StopRtAudio())
        {
            _inputDevice = device;
        }
    }

    return Success;
}

Result<> AudioEngine::SetOutputDevice(const AudioDevice& device)
{
    if (device.supported == false)
        return MAKE_ERROR(DeviceNotSupported);

    return Success;
}

Result<> AudioEngine::SetMaxPolyphony(U32 maxPolyphony)
{
    _maxPolyphony = maxPolyphony;
    return Success;
}

Result<> AudioEngine::Ignite()
{
    return Success;
}

void AudioEngine::AudioCallback(void* outputBuffer, void* inputBuffer, U32 nFrames, F64 streamTime, RtAudioStreamStatus status, void* userData)
{
    UNUSED(outputBuffer);
    UNUSED(inputBuffer);
    UNUSED(nFrames);
    UNUSED(streamTime);
    UNUSED(status);
    UNUSED(userData);
}

} // namespace ScmAudio