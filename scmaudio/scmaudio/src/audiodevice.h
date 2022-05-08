#pragma once

#include "types.h"
#include "constants.h"

namespace ScmAudio
{

struct AudioDevice
{
    AudioDevice()
        : id(InvalidId)
        , sampleRate(0)
        , channels(0)
        , bufferSize(0)
        , input(false)
        , name()
    {}

    bool operator==(const AudioDevice& other) const
    {
        return id == other.id
            && sampleRate == other.sampleRate
            && channels == other.channels
            && bufferSize == other.bufferSize
            && input == other.input
            && name == other.name;
    }

    U32 id;
    U32 sampleRate;
    U32 channels;
    U32 bufferSize;
    bool input;
    String name;
};

} // namespace ScmAudio