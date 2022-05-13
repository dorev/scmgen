#include "soundstore.h"
#include "utils.h"

namespace ScmAudio
{

Result<SoundId> SoundStore::Load(const String& path)
{
    if (_sounds.find(Hash(path)) != _sounds.end())
        return MAKE_ERROR(SoundAlreadyLoaded);

    if (!_loader.IsFileSupported(path))
        return MAKE_ERROR(FileNotSupported);

    AudioDataPtr soundData(new AudioData);
    try
    {
        _loader.Load(soundData.get(), path);
    }
    catch (std::exception& /*exception*/)
    {
        return MAKE_ERROR(UnableToLoadSound);
    }

    auto emplaceResult = _sounds.emplace(Hash(path), Sound(path, soundData));
    if (emplaceResult.second)
        return emplaceResult.first->second.GetId();
    else
        return MAKE_ERROR(UnableToStoreSound);
}

const Sound& SoundStore::GetSound(SoundId id) const
{
    auto it = _sounds.find(id);
    if (it == _sounds.end())
    {
        static const Sound stub;
        return stub;
    }

    return it->second;
}

const Sound& SoundStore::GetSound(const String& path) const
{
    return GetSound(Hash(path));
}

} // namespace ScmAudio
