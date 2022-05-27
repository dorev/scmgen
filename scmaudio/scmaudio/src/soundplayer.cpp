#include "soundplayer.h"
#include "utils.h"

namespace ScmAudio
{

SoundPlayer::SoundPlayer(U32 /*samplingRate*/, U32 maxPolyphony, U32 channels)
//    : _samplingRate(samplingRate)
    : _channels(channels)
    , _maxPolyphony(maxPolyphony)
    , _activeSoundsCount(0)
    , _activeSounds(_maxPolyphony)
    , _soundInstancePool(2 * _maxPolyphony)
{
}

Result<SoundInstancePtr> SoundPlayer::AddSoundInstance(const Sound& sound)
{
    SoundInstancePtr soundInstancePtr = _soundInstancePool.ConstructRefCountedObject(&sound);
    return StoreActiveSoundInstance(soundInstancePtr);
}

Result<SoundInstancePtr> SoundPlayer::StoreActiveSoundInstance(SoundInstancePtr soundInstancePtr)
{
    if (soundInstancePtr == nullptr)
        return MAKE_ERROR(UnableToStoreSound);

    for (U32 i = 0; i < _maxPolyphony; ++i)
    {
        if (_activeSounds[i] == nullptr)
        {
            _activeSoundsCount++;
            _activeSounds[i] = soundInstancePtr;
            return soundInstancePtr;
        }
    }

    return MAKE_ERROR(MaxPolyphonyReached);
}

void SoundPlayer::WriteFrames(void* outputBuffer, U32 framesToWrite)
{
    if (_activeSoundsCount == 0)
        return;

    F32* buffer = ToPtr<F32>(outputBuffer);
    U32 writtenInstancesCount = 0;

    for (SoundInstancePtr& soundInstancePtr : _activeSounds)
    {
        if (soundInstancePtr == nullptr)
            continue;

        SoundInstance& soundInstance = *soundInstancePtr;
        U32 remainingSamples = soundInstance.RemainingSamples();

        // Discard fully written instances
        if (remainingSamples == 0)
        {
            soundInstancePtr = nullptr;
            _activeSoundsCount--;
            continue;
        }

        const U32 outputChannels = GetChannels();
        const U32 soundChannels = soundInstance.GetSound().GetChannels();
        U32 remainingSoundFrames = remainingSamples / soundChannels;
        U32 soundSamplesToWrite = std::min(remainingSoundFrames, framesToWrite) * soundChannels;
        const F32* soundData = soundInstance.GetDataBegin();

        // Sound and output channel count match
        if (outputChannels == soundChannels)
        {
            for (U32 sample = 0; sample < soundSamplesToWrite; ++sample)
                buffer[sample] += soundData[sample];
        }
        // Output has more channels than sound
        // Round-robin sound channels into output channels
        else if (outputChannels > soundChannels)
        {
            for (U32 frame = 0; frame < framesToWrite; ++frame)
            {
                for (U32 outputChannel = 0; outputChannel < outputChannels; outputChannel++)
                {
                    U32 bufferSampleIndex = (frame * outputChannels) + outputChannel;
                    U32 soundSampleIndex = frame + (outputChannel % soundChannels);
                    buffer[bufferSampleIndex] += soundData[soundSampleIndex];
                }
            }
        }
        // Sound has more channels than output
        // Merge sound channels to mono and replicate on output channels (to improve...)
        else if (soundChannels > outputChannels)
        {
            for (U32 frame = 0; frame < framesToWrite; ++frame)
            {
                F32 mergedSample = 0.0f;

                for (U32 soundChannel = 0; soundChannel < soundChannels; soundChannel++)
                {
                    U32 soundSampleIndex = (frame * soundChannels) + soundChannel;
                    mergedSample += soundData[soundSampleIndex];
                }

                mergedSample /= soundChannels;

                for (U32 outputChannel = 0; outputChannel < outputChannels; outputChannel++)
                {
                    U32 bufferSampleIndex = (frame * outputChannels) + outputChannel;
                    buffer[bufferSampleIndex] += mergedSample;
                }
            }
        }

        soundInstance.IncrementPosition(soundSamplesToWrite);

        if ((++writtenInstancesCount) == _activeSoundsCount)
            break;
    }
}

} // namespace ScmAudio
