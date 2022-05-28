#include "audiobuffer.h"

namespace ScmAudio
{

AudioBuffer::AudioBuffer(U32 size, U32 channels, U32 frames)
    : channels(channels)
    , frames(frames)
{
    if (size > 0)
        samples.reserve(size);
}

void AudioBuffer::Clear()
{
    channels = 0;
    frames = 0;
    samples.clear();
}

void AudioBuffer::MonoFrom(const AudioBuffer& other)
{
    samples.resize(other.frames);
    samples.shrink_to_fit();
    frames = other.frames;
    channels = 1;

    if (other.channels > 1)
    {
        for (U32 frame = 0; frame < frames; frame++)
        {
            F32 monoSample = 0;
            for (U32 channel = 0; channel < channels; channel++)
                monoSample += other.samples[frame + other.channels];
            samples[frame] = monoSample;
        }
    }
}

} // namespace ScmAudio
