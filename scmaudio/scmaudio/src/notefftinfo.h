#pragma once

#include "defines.h"

namespace ScmAudio
{

class AudioFft;

class SCMAUDIO_API NoteFftInfo
{
public:
    static const U32 NoteLimit = 116;

public:
    NoteFftInfo(const AudioFft& audioFft, F32 a4Frequency = 440.0f);

    bool CheckNoteBinsAreUnique() const;

    const Vector<U32>& GetNoteFftBins() const { return _noteFftBins; }
    const Vector<F32>& GetNoteFrequencies() const { return _noteFrequencies; }

private:
    const AudioFft& _audioFft;
    F32 _a4Frequency;
    //U32 _outputBinCount;
    //F32 _binRange;
//#pragma warning(push)
//#pragma warning(disable:4251)
    Vector<F32> _noteFrequencies;
    Vector<U32> _noteFftBins;
//#pragma warning(pop)
};

} // namespace ScmAudio
