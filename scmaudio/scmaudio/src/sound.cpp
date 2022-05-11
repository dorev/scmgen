#pragma once

#include "sound.h"
#include "utils.h"

namespace ScmAudio
{

Sound::Sound()
    : _id(0)
    , _path("")
    , _data(nullptr)
    , _status(Idle)
{
}

Sound::Sound(const String& path, const AudioDataPtr& data)
    : _id(Hash(path))
    , _path(path)
    , _data(data)
    , _status(Idle)
{
}

const Vector<F32>& Sound::GetData() const
{
    static const Vector<F32> stub;
    if (HasData())
        return _data->samples;
    else
        return stub;
}

SoundId Sound::GetId() const
{
    return _id;
}

U32 Sound::GetSize() const
{
    if (HasData())
        return U32(_data->samples.size());
    else
        return 0;
}

const String& Sound::GetPath() const
{
    return _path;
}

Sound::Status Sound::GetStatus() const
{
    return _status;
}

bool Sound::HasData() const
{
    return _data != nullptr;
}

} // namespace ScmAudio