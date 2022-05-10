#pragma once

#include "audioengine.h"

namespace ScmAudio
{

int ScmAudioCallback(void* outputBuffer, void* inputBuffer, U32 nFrames, F64 streamTime, RtAudioStreamStatus status, void* userData)
{
    ASSERT(userData != nullptr);
    AudioEngine& engine = *reinterpret_cast<AudioEngine*>(userData);

    UNUSED(outputBuffer);
    UNUSED(inputBuffer);
    UNUSED(nFrames);
    UNUSED(streamTime);
    UNUSED(status);
    UNUSED(engine);
    return 0;
}

} // namespace ScmAudio