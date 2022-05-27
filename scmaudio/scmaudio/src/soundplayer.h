#pragma once

#include "defines.h"
#include "soundinstance.h"
#include "result.h"
#include "types.h"
#include "sound.h"
#include "audiodevice.h"

namespace ScmAudio
{

class SoundPlayer
{
public:
    DELETE_DEFAULT_CONSTRUCTOR(SoundPlayer);
    SoundPlayer(U32 samplingRate = 44100, U32 maxPolyphony = 32, U32 channels = 2);

    Result<SoundInstancePtr> AddSoundInstance(const Sound& sound);
    void WriteFrames(void* outputBuffer, U32 frameCount);

    void SetChannels(U32 channels) { _channels = channels; }
    U32 GetChannels() const { return _channels; }
    U32 GetActiveSoundsCount() const { return _activeSoundsCount; }

private:
    Result<SoundInstancePtr> StoreActiveSoundInstance(SoundInstancePtr soundInstancePtr);

private:
//    U32 _samplingRate;
    U32 _channels;
    U32 _maxPolyphony;
    U32 _activeSoundsCount;
    Vector<SoundInstancePtr> _activeSounds;
    LockfreeObjectPool<SoundInstance> _soundInstancePool;
};

} // namespace ScmAudio
