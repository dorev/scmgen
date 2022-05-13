#pragma once

#include "audioengine.h"
#include "defines.h"

namespace ScmAudio
{

int AudioCallback(void* outputBuffer, void* inputBuffer, U32 frameCount, F64 streamTime, RtAudioStreamStatus status, void* userData)
{
    ASSERT(userData != nullptr);
    if (status != 0)
        std::cout << '*';

    AudioEngine& engine = *reinterpret_cast<AudioEngine*>(userData);
    SoundPlayer& player = engine.GetPlayer();

    U32 outputBufferSize = engine.GetInputDevice().inputChannels * sizeof(F32) * frameCount;
    memset(outputBuffer, 0, outputBufferSize);

    if (!engine.IsMuted())
        player.WriteFrames(outputBuffer, frameCount);

    if (engine.IsCapturing())
    {
        if (player.GetActiveSoundsCount() > 0)
        {
            for (U32 frame = 0; frame < frameCount; frame++)
                reinterpret_cast<F32*>(outputBuffer)[frame] += reinterpret_cast<F32*>(inputBuffer)[frame];
        }
        else
        {
            memcpy(outputBuffer, inputBuffer, outputBufferSize);
        }
    }

    UNUSED(streamTime);
    return 0;
}

} // namespace ScmAudio`