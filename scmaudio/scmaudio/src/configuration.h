#pragma once

#include "types.h"

namespace ScmAudio
{
    struct Configuration
    {
        float notePresenceThreshold; // dB
        float beatDetectionThreshold; // dB
        uint32 beatDetectionWindow; // ms
        uint32 fftWindow; // samples
    };

} // namespace ScmAudio