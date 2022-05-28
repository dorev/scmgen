#include <numeric>

#include "fftstagingbuffer.h"
#include "logger.h"

namespace ScmAudio
{

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

} // namespace ScmAudio
