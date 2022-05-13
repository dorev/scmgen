#include "sound.h"
#include "utils.h"

namespace ScmAudio
{

Sound::Sound()
    : _id(0)
    , _path("")
    , _size(0)
    , _samplingRate(0)
    , _channels(0)
    , _format(Format::Invalid)
    , _status(Status::Idle)
    , _data(nullptr)
{
}

Sound::Sound(const String& path, const AudioDataPtr& data)
    : _id(Hash(path))
    , _path(path)
    , _size(0)
    , _samplingRate(0)
    , _channels(0)
    , _format(Format::Invalid)
    , _status(Status::Idle)
    , _data(data)
{
    if (_data != nullptr)
    {
        _size = U32(_data->samples.size());
        _samplingRate = U32(_data->sampleRate);
        _channels = U32(_data->channelCount);
        _format = static_cast<Sound::Format>(_data->sourceFormat);
    }
}

const Vector<F32>& Sound::GetData() const
{
    static const Vector<F32> stub;
    return IsValid() ? _data->samples : stub;
}

} // namespace ScmAudio
