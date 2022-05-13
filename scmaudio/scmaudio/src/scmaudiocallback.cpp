#pragma once

#include "audioengine.h"
#include "defines.h"
#include "utils.h"

namespace ScmAudio
{

int AudioCallback(void* outputBuffer, void* inputBuffer, U32 frameCount, F64 streamTime, RtAudioStreamStatus status, void* userData)
{
    ASSERT(userData != nullptr);
    if (status != 0)
        std::cout << '*';

    AudioEngine& engine = *reinterpret_cast<AudioEngine*>(userData);
    SoundPlayer& player = engine.GetPlayer();
    U32 inputChannels = engine.GetInputDevice().inputChannels;
    U32 outputChannels = engine.GetOutputDevice().outputChannels;

    U32 outputBufferSize = inputChannels * sizeof(F32) * frameCount;
    memset(outputBuffer, 0, outputBufferSize);

    if (!engine.IsMuted())
        player.WriteFrames(outputBuffer, frameCount);

    if (engine.IsCapturing())
    {
        if (player.GetActiveSoundsCount() > 0 || inputChannels != outputChannels)
        {
            F32* castOutputBuffer = ToPtr<F32>(outputBuffer);
            F32* castInputBuffer = ToPtr<F32>(inputBuffer);

            for (U32 frame = 0; frame < frameCount; ++frame)
            {
                for (U32 outputChannel = 0; outputChannel < outputChannels; outputChannel++)
                {
                    U32 outputSampleIndex = (frame * outputChannels) + outputChannel;
                    U32 inputSampleIndex = (frame * inputChannels) + (outputChannel % inputChannels);
                    castOutputBuffer[outputSampleIndex] += castInputBuffer[inputSampleIndex];
                }
            }
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