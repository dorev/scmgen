#pragma once

#include "types.h"
#include <RtAudio.h>

namespace ScmAudio
{

struct Empty {};
template <class T = Empty>
class Result
{
};

using AudioInstruction = uint32;
namespace AudioInstructions
{
    constexpr uint32 None = 0x00000000;
    constexpr uint32 Now = 0x00000001;
    constexpr uint32 Loop = 0x00000002;
    constexpr uint32 Linear = 0x00000003;
}

using SoundId = uint32;
using SoundPath = String;
constexpr uint32 InvalidSoundId = uint32(-1);

class AudioEngine;

class Sound
{
public:
    Sound(AudioEngine& engine, const SoundPath& path)
        : _engine(engine)
    {
    }

    Result<> Unload();
    Sound& Play(AudioInstruction = AudioInstructions::Now);
    Sound& Fade(uint32, AudioInstruction = AudioInstructions::Linear);

private:
    AudioEngine& _engine;
    SoundPath _path;
    SoundId _id;
    Vector<float> _samples;
};

using SoundInstanceId = uint32;
class SoundInstance
{
public:

private:
    SoundInstanceId _id;
    SoundId _soundId;
};

class AudioEngine
{
public:
    struct SessionReport;
    
    enum class Status
    {
        Uninitialized,
        Initializing,
        Initialized,
        Running,
        ShuttingDown
    };

    AudioEngine(uint32 maxPolyphony = 64)
        : _maxPolyphony(maxPolyphony)
        , _status(Status::Uninitialized)
    {
        /* Fun API project
        using namespace AudioInstructions;
        AudioEngine engine;
        Result<> result = engine.Load("*.mp3").Play().Unload();
        engine.PlayLoaded(0x00003DFF, Loop).Fade(1200, Linear);
        */
    }

    Sound& Load(String);
    Result<> Unload(SoundId);

    Result<> Unload(Sound&);
    Sound& GetSound(SoundId);
    Sound& GetSound(SoundPath);

    Sound& PlayLoaded(const Sound&, AudioInstruction = AudioInstructions::Now);
    Sound& PlayLoaded(SoundId, AudioInstruction = AudioInstructions::Now);
    Sound& PlayLoaded(SoundPath, AudioInstruction = AudioInstructions::Now);

    AudioEngine& StopAll();

    AudioEngine& Ignite(); // "Initialize" is so boring...
    SessionReport Shutdown();


private:
    void AudioCallback(void* outputBuffer, void* inputBuffer,
        unsigned int nFrames,
        double streamTime,
        RtAudioStreamStatus status,
        void* userData)
    {

    }

private:
    uint32 _maxPolyphony;
    Status _status;
    RtAudio _rtAudio;
};

} // namespace ScmAudio