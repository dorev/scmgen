#pragma once

#include "types.h"

namespace ScmAudio
{
    struct AudioDevice
    {
        String name;
        bool input;
        uint32 id;
        uint32 sampleRate;
        uint32 channels;
        uint32 bufferSize;
    };

} // namespace ScmAudio