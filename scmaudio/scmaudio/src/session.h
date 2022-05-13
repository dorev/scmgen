#pragma once

#include "defines.h"
#include "types.h"

namespace ScmAudio
{

struct AudioDevice;
struct SnapshotData;
struct Configuration;

class SCMAUDIO_API Session
{
public:

    String Initialize();
    Vector<AudioDevice> GetCaptureDevices() const;
    Vector<AudioDevice> GetPlaybackDevices() const;
    String SetPlaybackDevice(const AudioDevice&);
    String SetSource(const AudioDevice&);
    String SetSource(const String& /*filePath*/);
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Configure(const Configuration&);
    void ReadSnapshot(SnapshotData&) const;
    void TapTempo();
};

} // namespace ScmAudio