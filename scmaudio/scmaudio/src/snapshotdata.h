#pragma once

namespace ScmAudio
{

struct SnapshotData
{
    F32 notePresence[12];
    F32 octaveEnergy[8];
    F32 tempo;
    F32 peakVolume;
};

} // namespace ScmAudio