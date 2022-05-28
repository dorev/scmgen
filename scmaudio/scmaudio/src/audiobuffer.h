#pragma once

#include "types.h"

namespace ScmAudio
{

struct AudioBuffer
{
    U32 channels;
    U32 frames;
    Vector<F32> samples;

    AudioBuffer(U32 size = 0, U32 channels = 0, U32 frames = 0);
    void Clear();
    void MonoFrom(const AudioBuffer& other);
};

} // namespace ScmAudio
