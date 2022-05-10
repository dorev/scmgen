#pragma once

#include "types.h"
#include "constants.h"
#include "defines.h"
#include "RtAudio.h"

namespace ScmAudio
{

struct SCMAUDIO_API AudioDevice
{
    enum class Flow
    {
        Invalid,
        Input,
        Output,
        Duplex
    };

    U32 id;
    bool supported;
    Flow flow;
    U32 preferredSampleRate;
    U32 inputChannels;
    U32 outputChannels;
#pragma warning(push)
#pragma warning(disable:4251)
    Vector<U32> supportedSampleRates;
    String name;
#pragma warning(pop)

    AudioDevice()
        : id(InvalidId)
        , supported(false)
        , flow(Flow::Invalid)
        , inputChannels(0)
        , outputChannels(0)
        , name("")
        , preferredSampleRate(0)
    {}

    AudioDevice(const RtAudio::DeviceInfo& rtAudioDevice, U32 deviceIndex)
        : id(InvalidId)
        , supported(false)
        , flow(Flow::Invalid)
        , inputChannels(0)
        , outputChannels(0)
        , name("")
        , preferredSampleRate(0)
    {
        if (rtAudioDevice.probed == false)
            return;

        id = deviceIndex;
        name = rtAudioDevice.name;
        supported = (rtAudioDevice.nativeFormats & RTAUDIO_FLOAT32) != 0;
        inputChannels = rtAudioDevice.inputChannels;
        outputChannels = rtAudioDevice.outputChannels;

        preferredSampleRate = rtAudioDevice.preferredSampleRate;
        for (U32 sampleRate : rtAudioDevice.sampleRates)
            supportedSampleRates.push_back(sampleRate);

        if (inputChannels == 0 && outputChannels > 0)
            flow = Flow::Output;
        else if (inputChannels > 0 && outputChannels == 0)
            flow = Flow::Input;
        else
            flow = Flow::Duplex;
    }

    bool operator==(const AudioDevice& other) const
    {
        return id == other.id
            && supported == other.supported
            && flow == other.flow
            && preferredSampleRate == other.preferredSampleRate
            && inputChannels == other.inputChannels
            && outputChannels == other.outputChannels
            && name == other.name;
    }

    bool operator!=(const AudioDevice& other) const
    {
        return !(*this == other);
    }

    String GetFlowString() const
    {
        switch (flow)
        {
        case Flow::Input: return "Input";
        case Flow::Output: return "Output";
        case Flow::Duplex: return "Duplex";
        default: return "Invalid";
        }
    }

    bool IsValid()
    {
        return id != InvalidId && flow != Flow::Invalid;
    }
};

} // namespace ScmAudio