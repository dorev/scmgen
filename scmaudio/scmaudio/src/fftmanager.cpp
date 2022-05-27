#include "fftmanager.h"
#include "logger.h"

namespace ScmAudio
{

// Audio buffer

AudioBuffer::AudioBuffer(U32 size, U32 channels, U32 frames)
    : channels(channels)
    , frames(frames)
{
    if (size > 0)
        samples.reserve(size);
}

void AudioBuffer::Clear()
{
    channels = 0;
    frames = 0;
    samples.clear();
}

void AudioBuffer::MonoFrom(const AudioBuffer& other)
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

//  FftStagingBuffer

FftStagingBuffer::FftStagingBuffer(U32 blockSize, U32 outputBufferSize)
    : _blockSize(blockSize)
    , _stagingBufferCount(0)
    , _head(0)
    , _tail(0)
    , _outputBufferSize(outputBufferSize)
    , _stagingBuffers(_blockSize + 1)
{
}

AudioBuffer& FftStagingBuffer::GetNextToFill()
{
    if (!Full())
        _stagingBufferCount++;
    else
        _tail++;
    return _stagingBuffers[_head++ % _stagingBuffers.size()];
}

const AudioBuffer& FftStagingBuffer::Peek(U32 index, bool& success)
{
    success = !Empty() && index < _stagingBufferCount;
    return _stagingBuffers[(_tail + index) % _stagingBuffers.size()];
}

bool FftStagingBuffer::Empty() const
{
    return _head == _tail;
}

bool FftStagingBuffer::Full() const
{
    return (_head + 1) % _stagingBuffers.size() == _tail;
}

U32 FftStagingBuffer::TotalSamples()
{
    U32 indexToClear = (_head + 1) % _stagingBuffers.size();
    _stagingBuffers[indexToClear].Clear();

    return std::reduce(_stagingBuffers.begin(), _stagingBuffers.end(), 0,
        [](U32 sum, const AudioBuffer& buffer) { return sum + U32(buffer.samples.size()); });
}

bool FftStagingBuffer::FillOutputBuffer(Vector<F32>& outputBuffer)
{
    if (!Full() || TotalSamples() != outputBuffer.size())
        return false;

    if (outputBuffer.capacity() != _outputBufferSize)
        outputBuffer.resize(_outputBufferSize);

    for (U32 i = 0; i < (_stagingBuffers.size() - 1); i++)
    {
        bool peeked = false;
        const AudioBuffer& block = Peek(i, peeked);
        if (peeked)
            memcpy(outputBuffer.data() + (i * _blockSize), block.samples.data(), _blockSize);
        else
        {
            LOG_FFT_ERROR("Unable to full FFT buffer from audio buffers collected. Some buffers are likely missing or not fully filled");
            break;
        }
    }
    return false;
}

// FftManager

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
