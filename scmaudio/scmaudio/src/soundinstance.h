#pragma once

#include "defines.h"
#include "sound.h"
#include "types.h"
#include "objectpool.h"

namespace ScmAudio
{

class SoundInstance : public Poolable<SoundInstance>
{
public:
    SoundInstance();
    SoundInstance(const Sound& sound);
    const F32* GetDataBegin() const;
    U32 SamplesLeftToPlay() const;
    void SamplesPlayed(U32 samplesPlayed);

private:
    const Sound* _sound;
    U32 _position;
};

} // namespace ScmAudio