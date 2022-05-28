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

    Vector<AudioDevice> ListCaptureDevices() const;
    Vector<AudioDevice> ListPlaybackDevices() const;
    String SetPlaybackDevice(const AudioDevice&);
    String SetSource(const AudioDevice&);
    String SetSource(const String&);
    String SetSource(const void*, U32);
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void ReadSnapshot(SnapshotData&) const;
};

} // namespace ScmAudio
