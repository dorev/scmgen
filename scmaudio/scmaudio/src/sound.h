#pragma once

#include "types.h"
#include "result.h"

namespace ScmAudio
{

using SoundId = U32;
using SoundInstanceId = U32;

class SoundInstance
{
public:

private:
    SoundInstanceId _id;
    SoundId _soundId;
};

class AudioEngine;

class Sound
{
public:
    Sound(AudioEngine& engine, const String& path)
        : _engine(engine)
    {
    }

    Result<> Unload();
    SoundInstance Play();

private:
    AudioEngine& _engine;
    String _path;
    SoundId _id;
    Vector<F32> _samples;
};

} // namespace ScmAudio