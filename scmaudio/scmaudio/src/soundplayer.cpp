#pragma once

#include "soundplayer.h"
#include "utils.h"

namespace ScmAudio
{

SoundPlayer::SoundPlayer(U32 samplingRate, U32 maxPolyphony, U32 channels)
    : _samplingRate(samplingRate)
    , _channels(channels)
    , _maxPolyphony(maxPolyphony)
    , _activeSounds(_maxPolyphony)
    , _activeSoundsCount(0)
    , _soundInstancePool(2 * _maxPolyphony)
{
}

Result<SoundInstancePtr> SoundPlayer::AddSoundInstance(const Sound& sound)
{
    SoundInstancePtr soundInstancePtr(_soundInstancePool.ConstructObject(&sound), _soundInstancePool.GetDeleter());
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

    for (SoundInstancePtr& soundInstancePtr : _activeSounds)
    {
        if (soundInstancePtr == nullptr)
            continue;

        SoundInstance& soundInstance = *soundInstancePtr;
        ASSERT(_channels == soundInstance.GetSound().GetChannels());

        U32 remainingFrames = soundInstance.RemainingSamples() / soundInstance.GetSound().GetChannels();
        if (remainingFrames == 0)
        {
            soundInstancePtr = nullptr;
            _activeSoundsCount--;
            continue;
        }

        U32 samplesToWrite = std::min(remainingFrames, framesToWrite) * _channels;
        const F32* soundData = soundInstance.GetDataBegin();

        for (U32 sample = 0; sample < samplesToWrite; ++sample)
            buffer[sample] += soundData[sample];

        soundInstance.IncrementPosition(samplesToWrite);
    }
}

} // namespace ScmAudio