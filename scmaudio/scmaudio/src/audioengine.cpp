#pragma once

#include "audioengine.h"

namespace ScmAudio
{

AudioEngine::AudioEngine()
    : _maxPolyphony(32)
    , _status(Status::Uninitialized)
    , _rtAudio(nullptr)
{
}

Vector<AudioDevice> ListAudioDevices()
{
    // http://web.mit.edu/carrien/Public/speechlab/marc_code/ADAPT_VC/rtaudio/doc/html/index.html
}

AudioEngine& AudioEngine::SetInputDevice(const AudioDevice& device)
{
    return *this;
}

AudioEngine& AudioEngine::SetOutputDevice(const AudioDevice& device)
{
    return *this;
}

AudioEngine& AudioEngine::SetMaxPolyphony(U32 maxPolyphony)
{
    return *this;
}

Result<> AudioEngine::Ignite()
{

    return *this;
}

} // namespace ScmAudio