#pragma once

#include "types.h"

namespace ScmAudio
{
    class AudioDevice;
    class SnapshotData;
    class Configuration;

    class Session
    {
    public:
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