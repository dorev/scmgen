#pragma once

#include "defines.h"
#include "sound.h"
#include "types.h"
#include "lockfreeobjectpool.h"

namespace ScmAudio
{

class SoundInstance
{
public:
    SoundInstance(const Sound* sound = nullptr);
    const F32* GetDataBegin() const;
    U32 RemainingSamples() const;
    void IncrementPosition(U32 frameCount);
    const Sound& GetSound() const;

private:
    const Sound* _sound;
    U32 _position;
};

using SoundInstancePtr = SharedPtr<SoundInstance>;

} // namespace ScmAudio
