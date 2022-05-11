#pragma once

#include "soundinstance.h"

namespace ScmAudio
{

SoundInstance::SoundInstance()
    : _sound(nullptr)
    , _position(0)
{
}

SoundInstance::SoundInstance(const Sound& sound)
    : _sound(&sound)
    , _position(0)
{
}

const F32* SoundInstance::GetDataBegin() const
{
    ASSERT(_sound != nullptr);
    return &_sound->GetData()[_position];
}

U32 SoundInstance::SamplesLeftToPlay() const
{
    ASSERT(_sound != nullptr);
    return _sound->GetSize() - _position;
}

void SoundInstance::SamplesPlayed(U32 sampleCount)
{
    ASSERT(_sound != nullptr);
    _position = std::min(_position + sampleCount, _sound->GetSize());
}

} // namespace ScmAudio