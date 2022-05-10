#pragma once

#include <RtAudio.h>

#include "types.h"

namespace ScmAudio
{

int ScmAudioCallback(void* outputBuffer, void* inputBuffer, U32 nFrames, F64 streamTime, RtAudioStreamStatus status, void* userData);

} // namespace ScmAudio