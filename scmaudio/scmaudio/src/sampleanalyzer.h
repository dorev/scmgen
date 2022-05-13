#pragma once

#include "types.h"
#include "fftwutils.h"
#include "notefftinfo.h"
#include "audiofft.h"

namespace ScmAudio
{

struct SCMAUDIO_API NoteValue
{
    U32 note;
    F32 value;
};

class SCMAUDIO_API SampleAnalyzer
{
public:
    SampleAnalyzer(U32 fftWindowSize, U32 samplingRate, F32 a4Frequency = 440.0f);
    Vector<NoteValue> GetNotesFromSamples(U32 resultSize);

    const NoteFftInfo& GetNoteFftInfo() const { return _noteFftInfo; }
    F32* GetInputBuffer() { return _audioFft.GetInputBuffer(); }
    U32 GetInputBufferSize() const { return _audioFft.GetInputBufferSize(); }

private:
    U32 _fftWindowSize;
    U32 _samplingRate;
    F32 _a4Frequency;
    AudioFft _audioFft;
    NoteFftInfo _noteFftInfo;
    FftwComplex* _fftOutputBuffer;
};

} // namespace ScmAudio
