#pragma once

#include <libnyquist/Common.h>

#include "types.h"

namespace ScmAudio
{

using SoundId = size_t;
using AudioData = nqr::AudioData;
using AudioDataPtr = SharedPtr<nqr::AudioData>;

class Sound
{
public:
    enum class Status
    {
        Idle,
        Playing,
        Unloading
    };

    enum class Format
    {
        U8,
        S8,
        S16,
        S24,
        S32,
        S64,
        F32,
        F64,
        Invalid
    };

public:
    Sound();
    Sound(const String& path, const AudioDataPtr& data);

    const Vector<F32>& GetData() const;

    SoundId GetId() const { return _id; }
    const String& GetPath() const { return _path; }
    U32 GetSize() const { return IsValid() ? _size : 0; }
    U32 GetChannels() const { return IsValid() ? _channels : 0; }
    U32 GetSamplingRate() const { return IsValid() ? _samplingRate : 0; }
    Sound::Format GetFormat() const { return IsValid() ? _format : Format::Invalid; }
    Sound::Status GetStatus() const { return _status; }
    bool IsValid() const { return _data != nullptr; }

private:
    SoundId _id;
    String _path;
    U32 _size;
    U32 _channels;
    U32 _samplingRate;
    Format _format;
    Status _status;
    AudioDataPtr _data;
};

} // namespace ScmAudio