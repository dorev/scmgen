#include "fftmanager.h"
#include "logger.h"

namespace ScmAudio
{

FftManager::FftManager(U32 fftWindowSize, U32 samplingRate, U32 audioBufferSize)
    : _fftWindowSize(fftWindowSize)
    , _samplingRate(samplingRate)
    , _audioBufferQueue(64)
    , _preallocatedBuffers(128, audioBufferSize)
    , _fftStagingBuffer(audioBufferSize, _fftWindowSize)
    , _sampleAnalyzer(_fftWindowSize, _samplingRate)
{
    StartThread();
}

FftManager::~FftManager()
{
    StopThread();
}

// Called from audio thread
void FftManager::StoreSamples(void* buffer, U32 frames, U32 channels)
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

bool FftManager::StartThread()
{
    if(_status == ThreadStatus::Idle)
    {
        _thread = Thread(&FftManager::ThreadFunction, this);
        _status = ThreadStatus::Running;
        return true;
    }
    return false;
}

bool FftManager::StopThread()
{
    if(_status == ThreadStatus::Running)
    {
        _status = ThreadStatus::Stopping;
        _thread.join();
        return true;
    }
    return false;
}

void FftManager::ThreadFunction()
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
                _fftStagingBuffer.FillOutputBuffer(_fftBuffer);
                LOG(Fatal, Fft, "Success!", "Great victory!");
            }
            else
            {
                LOG_FFT_ERROR("Mismatch in sample count to fill FFT buffer");
                continue;
            }
        }
        else
        {
            LOG_FFT_ERROR("Not ready to fill FFT buffer");
            continue;
        }

    }
}

} // namespace ScmAudio
