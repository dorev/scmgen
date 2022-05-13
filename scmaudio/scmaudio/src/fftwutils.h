#pragma once

#include <fftw3.h>
#include "types.h"
#include "defines.h"

namespace ScmAudio
{

using FftwReal = F32;
using FftwComplex = fftwf_complex;
using FftwPlan = fftwf_plan;
#define FftwFree fftwf_free
#define FftwMalloc fftwf_malloc
#define FftwDftPlan fftwf_plan_dft_r2c_1d
#define FftwDestroyPlan fftwf_destroy_plan
#define FftwExecute fftwf_execute
#define FftwExecuteDft fftwf_execute_dft_r2c

inline void HammingWindow(F32* buffer, U32 bufferSize)
{
    static const F32 a0 = 25.0f / 46.0f;
    static const F32 a1 = 1.0f - a0;
    for (U32 i = 0; i < bufferSize; ++i)
        buffer[i] *= a0 - a1 * cos(_2PI * i / bufferSize);
}

inline void HannWindow(F32* buffer, U32 bufferSize)
{
    for (U32 i = 0; i < bufferSize; ++i)
        buffer[i] = pow(sin((_PI * buffer[i]) / bufferSize), 2.0f);
}

inline void ScaleToDecibel(FftwComplex* buffer, U32 bufferSize)
{
    for (U32 i = 0; i < bufferSize; ++i)
        buffer[i][0] = 10.0f * log10(buffer[i][0] * buffer[i][0]);
}

} // namespace ScmAudio
