#pragma once

#include <RtAudio.h>

#include "types.h"

namespace ScmAudio
{

int AudioCallback(void* outputBuffer, void* inputBuffer, U32 frameCount, F64 streamTime, RtAudioStreamStatus status, void* userData);

} // namespace ScmAudio
