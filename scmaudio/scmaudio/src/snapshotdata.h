#pragma once

namespace ScmAudio
{

struct SnapshotData
{
    float notePresence[12];
    float octaveEnergy[8];
    float tempo;
    float peakVolume;
};

} // namespace ScmAudio