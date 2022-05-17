#pragma once

#include "types.h"
#include "defines.h"
#include <readerwritercircularbuffer.h>

namespace ScmAudio
{

struct SampleBlock
{
    U32 channels;
    Vector<F32> samples;
};

class SCMAUDIO_API FftManager
{
public:

    FftManager(U32 bufferSize, U32 fftWindowSize, U32 samplingRate);

    void StoreSamples(void* buffer, U32 bufferSize, U32 channels);
    void StartWorker();

    static void InputBufferMerger(FftManager& context);



private:

    template <class T>
    using LockfreeCircularBuffer = moodycamel::BlockingReaderWriterCircularBuffer<T>;

    LockfreeCircularBuffer<SampleBlock> _rawBuffersFromAudio;
    Vector<F32> _monoSamplesForFft;


};

} // namespace ScmAudio
