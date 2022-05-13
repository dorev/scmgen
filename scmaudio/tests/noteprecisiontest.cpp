
#include <fstream>

#include "gtest/gtest.h"
#include "testutils.h"
#include "fftwutils.h"
#include "audioutils.h"
#include "defines.h"
#include "utils.h"
#include "sampleanalyzer.h"

namespace ScmAudio
{

class FftSpeedTest : public testing::TestWithParam<std::tuple<F32, U32>>
{
};

// 44100 kHz - 16384 bins seems to be the best compromise
//const Vector<F32> standardSampleRates{ 44100, 48000, 88200, 96000 };
const Vector<F32> standardSampleRates{ 44100 };
const Vector<U32> standardFftWindowSizes{ 1024, 2048, 4096, 8192 };

INSTANTIATE_TEST_SUITE_P(SamplingRateAndFftWindowPermutationsSuite,
    FftSpeedTest,
    ::testing::Combine(
        ::testing::ValuesIn(standardSampleRates),
        ::testing::ValuesIn(standardFftWindowSizes)));

TEST_P(FftSpeedTest, FftAllSingleNotes)
{
    Stopwatch stopwatch;
    F32 samplingRate = std::get<0>(GetParam());
    U32 fftWindowSize = std::get<1>(GetParam());
    Milliseconds windowDuration = static_cast<Milliseconds>(fftWindowSize) / static_cast<Milliseconds>(samplingRate) * 1000.0;
    SampleAnalyzer s2n(fftWindowSize, samplingRate);
    const NoteFftInfo& noteInfo = s2n.GetNoteFftInfo();

    COUT_GTEST << "FFT SIZE: " << fftWindowSize << " - SAMPLING RATE : " << samplingRate << " Hz - " << ANSI_TXT_YLW << "WINDOW DURATION : " << windowDuration << " ms" ENDL;
    U32 successfulDetections = 0;
    Vector<U32> missedDetections;
    for (U32 note = 0; note < noteInfo.NoteLimit; ++note)
    {
        // Run FFT
        F32 noteFrequency = noteInfo.GetNoteFrequencies()[note];
        GenerateSinWave(s2n.GetInputBuffer(), fftWindowSize, noteFrequency, samplingRate);
        stopwatch.Start();
        Vector<NoteValue> result = s2n.GetNotesFromSamples(3);
        stopwatch.Lap();
        if (result.size() > 0 && result[0].note == note)
            ++successfulDetections;
        else
            missedDetections.push_back(note);
    }
    COUT_GTEST << "AVG SAMPLESTONOTES EXECUTION: " << stopwatch.GetLapsAverage() << " ms - SUCCESSFUL DETECTION: " << static_cast<double>(successfulDetections) / static_cast<double>(noteInfo.NoteLimit) * 100.0 << " %" << ENDL;
    if (missedDetections.size() > 0)
    {
        COUT_GTEST << "Missed notes: ";
        for (const auto& note : missedDetections)
            COUT << note << " (" << NoteNames[note % 12][0] << (note / 12) << ")  ";
        COUT << ENDL;
    }
}

} // namespace SamplesToNotes