#pragma once

#include "audioengine.h"
#include "defines.h"

namespace ScmAudio
{

int AudioCallback(void* outputBuffer, void* inputBuffer, U32 frameCount, F64 streamTime, RtAudioStreamStatus status, void* userData)
{
    ASSERT(userData != nullptr);
    //ASSERT(status == 0);
    if (status != 0)
        std::cout << '*';

    AudioEngine& engine = *reinterpret_cast<AudioEngine*>(userData);

    if (engine.GetInputDevice().inputChannels == engine.GetOutputDevice().outputChannels)
    {
        U32 channels = engine.GetInputDevice().inputChannels;
        memcpy(outputBuffer, inputBuffer, channels * sizeof(F32) * frameCount);
    }

    UNUSED(streamTime);
    UNUSED(status);
    return 0;
}

} // namespace ScmAudio`