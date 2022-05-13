#pragma once

#include "soundinstance.h"

namespace ScmAudio
{

SoundInstance::SoundInstance(const Sound* sound)
    : _sound(sound)
    , _position(0)
{
}

const F32* SoundInstance::GetDataBegin() const
{
    if (_sound != nullptr)
        return &_sound->GetData()[_position];
    else
        return nullptr;
}

U32 SoundInstance::RemainingSamples() const
{
    if (_sound != nullptr)
        return _sound->GetSize() - _position;
    else
        return 0;
}

void SoundInstance::IncrementPosition(U32 frameCount)
{
    if(_sound != nullptr)
        _position = std::min(_position + frameCount, _sound->GetSize());
}

const Sound& SoundInstance::GetSound() const
{
    if (_sound != nullptr)
        return *_sound;
    else
    {
        static const Sound stub;
        return stub;
    }
}

} // namespace ScmAudio