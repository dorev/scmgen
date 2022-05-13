#pragma once

#include "types.h"
#include "defines.h"

namespace ScmAudio
{


class SCMAUDIO_API FftManager
{
public:

    FftManager(U32 bufferSize, U32 fftWindowSize, U32 samplingRate);

    void StoreSamples(void* buffer, U32 bufferSize, U32 channels);
    void StartWorker();

    static void InputBufferMerger(FftManager& context);



private:

    // CircularBuffer<F32> _inputBuffer;
    // CircularBuffer<F32> _monoBuffer;



};

} // namespace ScmAudio
