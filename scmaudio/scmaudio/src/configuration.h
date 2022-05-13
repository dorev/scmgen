#pragma once

#include "types.h"

namespace ScmAudio
{

struct Configuration
{
    F32 notePresenceThreshold; // dB
    F32 beatDetectionThreshold; // dB
    U32 beatDetectionWindow; // ms
    U32 fftWindow; // samples
};

} // namespace ScmAudio
