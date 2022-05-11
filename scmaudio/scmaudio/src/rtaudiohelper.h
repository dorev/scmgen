#pragma once

#include <RtAudio.h>
#include "error.h"

namespace ScmAudio
{

struct RtAudioHelper
{
    static Error ErrorConverter(RtAudioErrorType error)
    {
        switch (error)
        {
        case RTAUDIO_WARNING: return MAKE_ERROR(WARNING);
        case RTAUDIO_UNKNOWN_ERROR: return MAKE_ERROR(UNKNOWN_ERROR);
        case RTAUDIO_NO_DEVICES_FOUND: return MAKE_ERROR(NO_DEVICES_FOUND);
        case RTAUDIO_INVALID_DEVICE: return MAKE_ERROR(INVALID_DEVICE);
        case RTAUDIO_DEVICE_DISCONNECT: return MAKE_ERROR(DEVICE_DISCONNECT);
        case RTAUDIO_MEMORY_ERROR: return MAKE_ERROR(MEMORY_ERROR);
        case RTAUDIO_INVALID_PARAMETER: return MAKE_ERROR(INVALID_PARAMETER);
        case RTAUDIO_INVALID_USE: return MAKE_ERROR(INVALID_USE);
        case RTAUDIO_DRIVER_ERROR: return MAKE_ERROR(DRIVER_ERROR);
        case RTAUDIO_SYSTEM_ERROR: return MAKE_ERROR(SYSTEM_ERROR);
        case RTAUDIO_THREAD_ERROR: return MAKE_ERROR(THREAD_ERROR);
        default:
            return MAKE_ERROR(Undefined);
        }
    }
};

} // namespace ScmAudio