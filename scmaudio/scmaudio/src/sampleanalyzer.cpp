#include "sampleanalyzer.h"

namespace ScmAudio
{

SampleAnalyzer::SampleAnalyzer(U32 fftWindowSize, U32 samplingRate, F32 a4Frequency)
    : _fftWindowSize(fftWindowSize)
    , _samplingRate(samplingRate)
    , _a4Frequency(a4Frequency)
    , _audioFft(_fftWindowSize, _samplingRate)
    , _noteFftInfo(_audioFft, _a4Frequency)
    , _fftOutputBuffer(_audioFft.GetOutputBuffer())
{
}

Vector<NoteValue> SampleAnalyzer::GetNotesFromSamples(U32 resultSize)
{
    _audioFft.Execute();
    auto topBins = _audioFft.GetTopBins(resultSize);

    // Scan note bins from the FFT output buffer
    Vector<NoteValue> results(resultSize, {0, 0});
    const Vector<U32>& noteBins = _noteFftInfo.GetNoteFftBins();

    for (U32 note = 0; note < NoteFftInfo::NoteLimit; ++note)
    {
        // Compare with each saved result
        for (U32 i = 0; i < resultSize; ++i)
        {
            U32 noteBin = noteBins[note];
            F32 value = _fftOutputBuffer[noteBin][0];
            if (value > results[i].value)
            {
                // Shift right lower values
                if (resultSize > 1)
                {
                    for (U32 j = resultSize - 1; j > i; --j)
                        results[j] = results[j - 1];
                }

                // Insert higher result
                results[i].note = note;
                results[i].value = value;
                break;
            }
        }
    }
    return results;
}

} // namespace ScmAudio
