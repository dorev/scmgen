#pragma once

#include "types.h"
#include "constants.h"
#include "defines.h"
#include "RtAudio.h"

namespace ScmAudio
{

struct SCMAUDIO_API AudioDevice
{
    enum Flow
    {
        Invalid,
        Input,
        Output,
        Duplex
    };

    String GetFlowString() const
    {
        switch (flow)
        {
        case Input: return "Input";
        case Output: return "Output";
        case Duplex: return "Duplex";
        default: return "Invalid";
        }
    }

    AudioDevice()
        : id(InvalidId)
        , supported(false)
        , flow(Invalid)
        , inputChannels(0)
        , outputChannels(0)
        , name("")
        , preferredSampleRate(0)
    {}

    AudioDevice(const RtAudio::DeviceInfo& rtAudioDevice, U32 deviceIndex)
    {
        if (rtAudioDevice.probed == false)
        {
            id = InvalidId;
            return;
        }

        id = deviceIndex;
        name = rtAudioDevice.name;
        supported = (rtAudioDevice.nativeFormats & RTAUDIO_FLOAT32) != 0;
        inputChannels = rtAudioDevice.inputChannels;
        outputChannels = rtAudioDevice.outputChannels;

        preferredSampleRate = rtAudioDevice.preferredSampleRate;
        for (U32 sampleRate : rtAudioDevice.sampleRates)
            supportedSampleRates.push_back(sampleRate);

        if (inputChannels == 0 && outputChannels > 0)
            flow = Output;
        else if (inputChannels > 0 && outputChannels == 0)
            flow = Input;
        else
            flow = Duplex;
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
};

} // namespace ScmAudio