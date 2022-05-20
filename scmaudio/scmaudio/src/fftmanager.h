#pragma once

#include <iostream>

#include "types.h"
#include "defines.h"
#include "utils.h"
#include "numeric"
#include "lockfreeobjectpool.h"
#include "sampleanalyzer.h"
#include <readerwritercircularbuffer.h>

namespace ScmAudio
{

struct AudioBuffer
{
    U32 channels = 0;
    U32 frames = 0;
    Vector<F32> samples;

    AudioBuffer(U32 size = 0, U32 channels = 0, U32 frames = 0)
        : channels(channels)
        , frames(frames)
    {
        if (size > 0)
            samples.reserve(size);
    }

    void Clear()
    {
        channels = 0;
        frames = 0;
        samples.clear();
    }

    void MonoFrom(const AudioBuffer& other)
    {
        samples.reserve(other.frames);
        frames = other.frames;
        channels = 1;

        if (other.channels > 1)
        {
            for (U32 frame = 0; frame < frames; frame++)
            {
                F32 monoSample = 0;
                for (U32 channel = 0; channel < channels; channel++)
                    monoSample += other.samples[frame + other.channels];
                samples[frame] = monoSample;
            }
        }
    }
};

class FftStagingBuffer
{
public:
    FftStagingBuffer(U32 blockSize, U32 outputBufferSize)
        : _blockSize(blockSize)
        , _stagingBufferCount(0)
        , _head(0)
        , _tail(0)
        , _stagingBuffers(_blockSize + 1)
        , _outputBuffer(outputBufferSize)
    {
    }

    AudioBuffer& GetNextToFill()
    {
        if (!Full())
            _stagingBufferCount++;
        else
            _tail++;
        return _stagingBuffers[_head++ % _stagingBuffers.size()];
    }

    const AudioBuffer& Peek(U32 index, bool& success)
    {
        success = !Empty() && index < _stagingBufferCount;
        return _stagingBuffers[(_tail + index) % _stagingBuffers.size()];
    }

    bool Empty() const { return _head == _tail; }
    bool Full() const { return (_head + 1) % _stagingBuffers.size() == _tail; }

    U32 TotalSamples()
    {
        U32 indexToClear = (_head + 1) % _stagingBuffers.size();
        _stagingBuffers[indexToClear].Clear();

        return std::reduce(_stagingBuffers.begin(), _stagingBuffers.end(), 0,
            [&](U32 sum, const AudioBuffer& buffer) { sum += buffer.samples.size(); }
        );
    }

    bool FillOutputBuffer(void* outputBuffer)
    {
        if (!Full() || TotalSamples() != _outputBuffer.size())
            return false;

        for (U32 i = 0; i < (_stagingBuffers.size() - 1); i++)
        {
            bool peeked = false;
            const AudioBuffer& block = Peek(i, peeked);
            if (peeked)
                memcpy(outputBuffer + i * _blockSize, block.samples.data(), _blockSize);
        }

    }

private:
    U32 _blockSize;
    U32 _stagingBufferCount;
    U32 _head;
    U32 _tail;
    Vector<AudioBuffer> _stagingBuffers;
    Vector<F32> _outputBuffer;
};

template <class T>
using LockfreeCircularBuffer = moodycamel::BlockingReaderWriterCircularBuffer<T>;

class SCMAUDIO_API FftManager
{
public:

    FftManager(U32 fftWindowSize, U32 samplingRate, U32 audioBufferSize)
        : _fftWindowSize(fftWindowSize)
        , _samplingRate(samplingRate)
        , _audioBufferQueue(64)
        , _preallocatedBuffers(128, audioBufferSize)
        , _fftStagingBuffer(fftWindowSize / audioBufferSize)
    {

    }

    // Called from audio thread
    void StoreSamples(void* buffer, U32 frames, U32 channels)
    {
        AudioBuffer* audioBufferPtr = _preallocatedBuffers.ObtainObject();
        if (audioBufferPtr != nullptr)
        {
            AudioBuffer& audioBuffer = *audioBufferPtr;
            audioBuffer.frames = frames;
            audioBuffer.channels = channels;
            U32 samples = frames * channels;
            if (samples == audioBuffer.samples.capacity())
            {
                memcpy(audioBuffer.samples.data(), buffer, samples);
                bool enqueueResult = _audioBufferQueue.try_enqueue(audioBufferPtr);
                UNUSED(enqueueResult);
            }
        }

    }

    bool StartThread()
    {
        if(_status == ThreadStatus::Idle)
        {
            _thread = Thread(&FftManager::Work, this);
            _status = ThreadStatus::Running;
            return true;
        }
        return false;
    }

    bool StopThread()
    {
        if(_status == ThreadStatus::Running)
        {
            _status = ThreadStatus::Stopping;
            _thread.join();
            return true;
        }
        return false;
    }

    void Work()
    {
        while(_status != ThreadStatus::Stopping)
        {
            static thread_local AudioBuffer* audioBuffer;

            while (_audioBufferQueue.try_dequeue(audioBuffer))
                _fftStagingBuffer.GetNextToFill().MonoFrom(*audioBuffer);

            if (_fftStagingBuffer.Full())
            {
                if (_fftStagingBuffer.TotalSamples() == _fftWindowSize)
                {


                }
                else
                {
                    std::cout << "Mismatch in sample count to fill FFT buffer\n";
                    continue;
                }
            }
            else
            {
                std::cout << "Not ready to fill FFT buffer\n";
                continue;
            }

        }
    }

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
