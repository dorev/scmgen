#include "session.h"
#include "audiodevice.h"
#include "configuration.h"
#include "snapshotdata.h"

namespace ScmAudio
{

Vector<AudioDevice> Session::GetCaptureDevices() const
{
    return {};
}

Vector<AudioDevice> Session::GetPlaybackDevices() const
{
    return {};
}

String Session::SetPlaybackDevice(const AudioDevice&)
{
    return "No error";
}

String Session::SetSource(const AudioDevice&)
{
    return "No error";
}

String Session::SetSource(const String& /*filePath*/)
{
    return "No error";
}

void Session::Start()
{

}

void Session::Stop()
{

}

void Session::Pause()
{

}

void Session::Resume()
{

}

void Session::Configure(const Configuration&)
{

}

void Session::ReadSnapshot(SnapshotData&) const
{

}

void Session::TapTempo()
{

}

} // namespace ScmAudio
