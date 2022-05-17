#pragma once

#include <fftw3.h>
#include "fftwutils.h"
#include "types.h"
#include "defines.h"

namespace ScmAudio
{

struct SCMAUDIO_API BinFrequency
{
    F32 low;
    F32 center;
    F32 high;
};

struct SCMAUDIO_API BinData
{
    U32 number;
    F32 value;
    BinFrequency frequency;
};

class SCMAUDIO_API AudioFft
{
public:
    AudioFft(U32 windowSize, U32 samplingRate);
    ~AudioFft();
    void Execute();
    void Execute(F32* buffer);
    BinData GetTopBin() const;
    Vector<BinData> GetTopBins(U32 topResults = 3) const;
    U32 GetBinForFrequency(F32 frequency) const;

    U32 GetWindowSize() const { return _windowSize; }
    F32 GetBinRange() const { return _binRange; }
    U32 GetInputBufferSize() const { return _windowSize; }
    U32 GetOutputBufferSize() const { return (_windowSize / 2) + 1; }
    F32* GetInputBuffer() { return _inputBuffer; }
    FftwComplex* GetOutputBuffer() const { return _outputBuffer; }
    const Vector<BinFrequency>& GetBinFrequencies() const { return _binFrequencies; }

private:
    U32 _windowSize;
    //U32 _samplingRate;
    F32 _binRange;
    F32* _inputBuffer;
    FftwComplex* _outputBuffer;
    FftwPlan _plan;
//#pragma warning(push)
//#pragma warning(disable:4251)
    Vector<BinFrequency> _binFrequencies;
//#pragma warning(pop)
};

} // namespace ScmAudio
