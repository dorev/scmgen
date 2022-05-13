#include <algorithm>

#include "audioutils.h"
#include "audiofft.h"
#include "notefftinfo.h"

namespace ScmAudio
{

NoteFftInfo::NoteFftInfo(const AudioFft& audioFft, F32 a4Frequency)
    : _audioFft(audioFft)
    , _a4Frequency(a4Frequency)
    , _noteFrequencies(NoteLimit)
    , _noteFftBins(NoteLimit)
{
    for (int i = 0; i < NoteLimit; ++i)
    {
        _noteFrequencies[i] = FrequencyOfNote(i, _a4Frequency);
        _noteFftBins[i] = _audioFft.GetBinForFrequency(_noteFrequencies[i]);
    }
}

bool NoteFftInfo::CheckNoteBinsAreUnique() const
{
    // Check if two notes are in the same bin at least once
    return std::adjacent_find(_noteFftBins.begin(), _noteFftBins.end()) == _noteFftBins.end();
}

} // namespace ScmAudio
