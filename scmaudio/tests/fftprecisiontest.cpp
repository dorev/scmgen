
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

class FftPrecisionTest : public testing::TestWithParam<std::tuple<F32, U32>>
{
};

// 44100 kHz - 16384 bins seems to be the best compromise
//const std::vector<F32> standardSampleRates{ 44100, 48000, 88200, 96000 };
const Vector<F32> standardSampleRates{ 44100 };
const Vector<U32> standardFftWindowSizes{ 1024, 2048, 4096, 8192 };

INSTANTIATE_TEST_SUITE_P(SamplingRateAndFftWindowPermutationsSuite,
    FftPrecisionTest,
    ::testing::Combine(
        ::testing::ValuesIn(standardSampleRates),
        ::testing::ValuesIn(standardFftWindowSizes)));

TEST_P(FftPrecisionTest, FftAllSingleNotePrecision)
{
    const bool verbose = false;
    F32 samplingRate = std::get<0>(GetParam());
    U32 fftWindowSize = std::get<1>(GetParam());
    Stopwatch stopwatch;

    Milliseconds windowDuration = static_cast<double>(fftWindowSize) / static_cast<double>(samplingRate) * 1000.0;
    if (verbose)
    {
        COUT_GTEST << "=================================================================================================" << ENDL;
        COUT_GTEST << "FFT SIZE: " << fftWindowSize << "\tSAMPLING RATE : " << samplingRate << " Hz\t" << ANSI_TXT_YLW << "WINDOW DURATION : " << windowDuration << " ms" ENDL;
        COUT_GTEST << "=================================================================================================" << ENDL;
    }
    else
    {
        COUT_GTEST <<  "FFT SIZE: " << fftWindowSize << "\tSAMPLING RATE : " << samplingRate << " Hz\t" << ANSI_TXT_YLW << "WINDOW DURATION : " << windowDuration << " ms" ENDL;
    }

    AudioFft fft(fftWindowSize, samplingRate);
    NoteFftInfo noteInfo(fft);
    U32 note = 0;

    struct FftTestResult
    {
        U32 note;
        F32 noteFrequency;
        F32 fftFrequency;
        F32 difference;
        F32 semitonePrecision;
        Milliseconds fftExecutionTime;
    };

    Vector<FftTestResult> results;
    results.reserve(NoteFftInfo::NoteLimit);

    // Execute FFT for all notes with generated sin wave input
    for (const F32 noteFrequency : noteInfo.GetNoteFrequencies())
    {
        if (verbose)
        {
            COUT_GTEST << "Note " << note << " (" << NoteNames[note % 12] << (note / 12) << ")" << "\tNote frequency: ";
            COUT << noteFrequency;
        }

        // Run FFT
        F32* inputBuffer = fft.GetInputBuffer();
        FftwComplex* outputBuffer = fft.GetOutputBuffer();
        GenerateSinWave(inputBuffer, fftWindowSize, noteFrequency, samplingRate);
        stopwatch.Start();
        fft.Execute();
        Milliseconds fftExecutionTime = stopwatch.Lap();
        F32 fftFrequency = fft.GetTopBin().frequency.center;
        if(verbose)
            COUT << "\tFFT frequency: " << std::setprecision(5) << fftFrequency;

        // Get difference between the desired note frequency and the frequency detected by FFT
        F32 difference = DifferencePercentage(noteFrequency, fftFrequency);
        if (verbose)
            COUT << "\tDifference: " << std::setprecision(5) << difference << "%";

        F32 deltaSign = fftFrequency < noteFrequency ? F32(-1) : F32(1);
        F32 semitonePrecision(0);
        // FFT frequency between the note and the next (except for last note)
        if (deltaSign > 0 && note < (NoteFftInfo::NoteLimit - 1))
        {
            F32 nextNoteFrequency = noteInfo.GetNoteFrequencies()[note + 1];
            semitonePrecision = (nextNoteFrequency - fftFrequency) / (nextNoteFrequency - noteFrequency) * F32(100);
        }
        // FFT frequency between the note and the previous  (except for first note)
        else if (deltaSign <= 0 && note > 0)
        {
            F32 previousNoteFrequency = noteInfo.GetNoteFrequencies()[note - 1];
            semitonePrecision = (fftFrequency - previousNoteFrequency) / (noteFrequency - previousNoteFrequency) * F32(100);
        }
        else
        {
            if (verbose)
                COUT << "\tUnable to calculate semitone precision" << ENDL;
            ++note;
            continue;
        }

        auto precisionColor = ANSI_TXT_DFT;
        if (semitonePrecision >= 99)
            precisionColor = ANSI_TXT_CYA;
        else if (semitonePrecision >= 90)
            precisionColor = ANSI_TXT_GRN;
        else if (semitonePrecision >= 80)
            precisionColor = ANSI_TXT_YLW;
        else
            precisionColor = ANSI_TXT_RED;
        if (verbose)
            COUT << precisionColor << "  Semitone precision: " << std::setprecision(5) << semitonePrecision << "%" << ENDL;

        // Store results
        results.push_back({ note, noteFrequency, fftFrequency, difference, semitonePrecision, fftExecutionTime });
        ++note;
    }

    U32 highestNoteDetected = results.size();
    U32 lowestNoteDetected = 0;
    for (U32 i = (NoteFftInfo::NoteLimit / 2); i < results.size(); ++i)
    {
        if (results[i].semitonePrecision < 80)
        {
            highestNoteDetected = i - 1;
            break;
        }
    }
    for (U32 i = (NoteFftInfo::NoteLimit / 2); i >= 0; --i)
    {
        if (results[i].semitonePrecision < 80)
        {
            lowestNoteDetected = i + 1;
            break;
        }
    }

    COUT_GTEST << "NOTE RANGE:\t"
        << lowestNoteDetected << " (" << NoteNames[lowestNoteDetected % 12][0] << (lowestNoteDetected / 12) << ") - "
        << highestNoteDetected << " (" << NoteNames[highestNoteDetected % 12][0] << (highestNoteDetected / 12) << ")\t"
        << "COVERAGE: " << F32(highestNoteDetected - lowestNoteDetected) / F32(NoteFftInfo::NoteLimit) * F32(100) << " %\t" 
        << "AVG FFT TIME: " << stopwatch.GetLapsAverage() << " ms" << ENDL;
}

} // namespace SamplesToNotes