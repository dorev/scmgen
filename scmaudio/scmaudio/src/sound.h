#pragma once

#include <libnyquist/Common.h>

#include "types.h"

namespace ScmAudio
{

using SoundId = size_t;
using AudioDataPtr = SharedPtr<nqr::AudioData>;

class Sound
{
public:
    Sound();
    Sound(const String& path, const AudioDataPtr& data);
    const Vector<F32>& GetData() const;
    SoundId GetId() const;
    U32 GetSize() const;
    const String& GetPath() const;
    bool HasData() const;

private:
    SoundId _id;
    String _path;
    AudioDataPtr _data;
};


} // namespace ScmAudio