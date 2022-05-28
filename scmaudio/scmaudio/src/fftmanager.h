#pragma once

#include <iostream>

#include "audiobuffer.h"
#include "fftstagingbuffer.h"
#include "types.h"
#include "lockfreeobjectpool.h"
#include "sampleanalyzer.h"

namespace ScmAudio
{

class SCMAUDIO_API FftManager
{
public:

    FftManager(U32 fftWindowSize, U32 samplingRate, U32 audioBufferSize);
    ~FftManager();

    // Called from audio thread
    void StoreSamples(void* buffer, U32 frames, U32 channels);
    bool StartThread();
    bool StopThread();

private:
    void ThreadFunction();

private:
    enum class ThreadStatus
    {
        Idle,
        Running,
        Stopping
    };
    
    ThreadStatus _status;
    U32 _fftWindowSize;
    U32 _samplingRate;
    Vector<F32> _fftBuffer;
    LockfreeCircularBuffer<AudioBuffer*> _audioBufferQueue;
    LockfreeObjectPool<AudioBuffer> _preallocatedBuffers;
    FftStagingBuffer _fftStagingBuffer;
    SampleAnalyzer _sampleAnalyzer;
    Thread _thread;
};

} // namespace ScmAudio
