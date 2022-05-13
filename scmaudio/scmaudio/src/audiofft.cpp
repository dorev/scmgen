
#include "audiofft.h"

namespace ScmAudio
{

AudioFft::AudioFft(U32 windowSize, U32 samplingRate)
    : _windowSize(windowSize)
    , _samplingRate(samplingRate)
    , _binRange(static_cast<F32>(samplingRate) / static_cast<F32>(GetInputBufferSize()))
    , _plan(FftwDftPlan(static_cast<U32>(_windowSize), _inputBuffer, _outputBuffer, FFTW_ESTIMATE))
    , _inputBuffer(nullptr)
    , _outputBuffer(nullptr)
    , _binFrequencies(GetOutputBufferSize())
{
    // Allocate arrays
    _inputBuffer = reinterpret_cast<F32*>(FftwMalloc(sizeof(F32) * GetInputBufferSize()));
    _outputBuffer = reinterpret_cast<FftwComplex*>(FftwMalloc(sizeof(FftwComplex) * GetOutputBufferSize()));

    // Set bins center frequency values
    for (U32 i = 0; i < GetOutputBufferSize(); ++i)
    {
        _binFrequencies[i].low = i * _binRange;
        _binFrequencies[i].center = (i * _binRange) + (_binRange / 2);
        _binFrequencies[i].high = (i + 1) * _binRange;
    }
}

AudioFft::~AudioFft()
{
    if (_inputBuffer)
    {
        FftwFree(_inputBuffer);
        _inputBuffer = nullptr;
    }
    if (_outputBuffer)
    {
        FftwFree(_outputBuffer);
        _outputBuffer = nullptr;
    }
    FftwDestroyPlan(_plan);
}

void AudioFft::Execute()
{
    HammingWindow(_inputBuffer, GetInputBufferSize());
    FftwExecuteDft(_plan, _inputBuffer, _outputBuffer);
}

void AudioFft::Execute(F32* buffer)
{
    HammingWindow(buffer, GetInputBufferSize());
    FftwExecuteDft(_plan, buffer, _outputBuffer);
}

U32 AudioFft::GetBinForFrequency(F32 frequency) const
{
    for (U32 bin = 0; bin < GetOutputBufferSize(); ++bin)
    {
        if (frequency >= _binFrequencies[bin].low
            && frequency < _binFrequencies[bin].high)
            return bin;
    }
    throw "Requested frequency is not within FFT range";
}

BinData AudioFft::GetTopBin() const
{
    return GetTopBins(1)[0];
}

Vector<BinData> AudioFft::GetTopBins(U32 topResults) const
{
    Vector<BinData> results(topResults, {0, 0, 0});

    // Scan whole output buffer
    for (U32 bin = 0; bin < GetOutputBufferSize(); ++bin)
    {
        // Compare with each saved result
        for (U32 i = 0; i < topResults; ++i)
        {
            F32 value = _outputBuffer[bin][0];
            BinData& result = results[i];
            if (value > result.value)
            {
                // Shift right lower values
                for (U32 j = topResults - 1; j > i; --j)
                    results[j] = results[j - 1];

                // Insert higher result
                result.number = bin;
                result.value = value;
                result.frequency = _binFrequencies[bin];
                break;
            }
        }
    }
    return results;
}


} // namespace ScmAudio
