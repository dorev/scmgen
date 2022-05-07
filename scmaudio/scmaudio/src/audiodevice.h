#pragma once

#include "types.h"

namespace ScmAudio
{
struct AudioDevice
{
    String name;
    bool input;
    uint32 id;
    uint32 sampleRate;
    uint32 channels;
    uint32 bufferSize;
};

struct Empty {};
template <class T = Empty>
class Result
{
};

using AudioInstruction = uint32;
namespace AudioInstructions
{
    constexpr uint32 None       = 0x00000000;
    constexpr uint32 Now        = 0x00000001;
    constexpr uint32 Loop       = 0x00000002;

    constexpr uint32 Linear     = 0x00000003;
}

class AudioEngine;

using SoundId = uint32;
using SoundName = String;
struct Sound
{
    Sound(AudioEngine& engine) : engine(engine) {}

    Sound& Play(SoundId);
    Sound& Play(SoundId, AudioInstruction);
    Sound& Play(SoundName);
    Sound& Play(SoundName, AudioInstruction);
    Result<> Unload();
    Sound& Fade(uint32);
    Sound& Fade(uint32, AudioInstruction);

    AudioEngine& engine;
};

class RefCountedLocklessPool
{
    struct SharedPointer;

};

class AudioEngine
{
public:
    struct SessionReport;
    enum Status;

    AudioEngine()
    {
        using namespace AudioInstructions;
        AudioEngine engine;

        Result<> result = engine.Load("*.mp3").Play(Now).Unload();
        engine.PlayLoaded(0x00003DFF, Loop).Fade(1200, Linear);
    }

    Sound& Load(String);
    Result<> Unload(SoundId);
    Result<> Unload(Sound&);
    Sound& GetSound(SoundId);
    Sound& GetSound(SoundName);

    Sound& PlayLoaded(const Sound&);
    Sound& PlayLoaded(const Sound&, AudioInstruction);
    Sound& PlayLoaded(SoundId);
    Sound& PlayLoaded(SoundId, AudioInstruction);
    Sound& PlayLoaded(SoundName);
    Sound& PlayLoaded(SoundName, AudioInstruction);

    AudioEngine& StopAll();

    AudioEngine& Ignite(); // "Initialize" is so boring...
    SessionReport Shutdown();


private:
    SessionReport Loop();

    uint32 _MaxPolyphony;
};


} // namespace ScmAudio