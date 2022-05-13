#pragma once

#include <libnyquist/Decoders.h>

#include "result.h"
#include "sound.h"
#include "types.h"

namespace ScmAudio
{

class SoundStore
{
public:
    Result<SoundId> Load(const String& path);
    const Sound& GetSound(SoundId id) const;
    const Sound& GetSound(const String& path) const;

private:
    UnorderedMap<SoundId, Sound> _sounds;
    nqr::NyquistIO _loader;
};

} // namespace ScmAudio
