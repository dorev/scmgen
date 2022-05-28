#pragma once

#include <iostream>

#include "audiobuffer.h"
#include "types.h"

namespace ScmAudio
{

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

} // namespace ScmAudio
