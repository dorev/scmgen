#pragma once

#include <iostream>

#include "types.h"
#include "defines.h"
#include "utils.h"
#include "numeric"
#include "lockfreeobjectpool.h"
#include "sampleanalyzer.h"

namespace ScmAudio
{

struct AudioBuffer
{
    U32 channels = 0;
    U32 frames = 0;
    Vector<F32> samples;

    AudioBuffer(U32 size = 0, U32 channels = 0, U32 frames = 0);
    void Clear();
    void MonoFrom(const AudioBuffer& other);
};

class FftStagingBuffer
{
public:
    FftStagingBuffer(U32 blockSize, U32 outputBufferSize);
    AudioBuffer& GetNextToFill();
    const AudioBuffer& Peek(U32 index, bool& success);
    bool Empty() const;
    bool Full() const;
    U32 TotalSamples();
    bool FillOutputBuffer(Vector<F32>& outputBuffer);

private:
    U32 _blockSize;
    U32 _stagingBufferCount;
    U32 _head;
    U32 _tail;
    U32 _outputBufferSize;
    Vector<AudioBuffer> _stagingBuffers;
};

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
