#include "gtest/gtest.h"
#include "audioengine.h"

namespace ScmAudio
{

class ScmAudioTests : public ::testing::Test
{
};

#define FAIL_ON_ERROR(result) if(result.HasError()) { FAIL() << result.GetError().GetDescription(); }

TEST_F(ScmAudioTests, DISABLED_ListDevices)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);

    auto deviceListResult = engine.ListAudioDevices();
    FAIL_ON_ERROR(deviceListResult);

    const auto deviceList = deviceListResult.GetValue();
    for (const auto& device : deviceList)
        std::cout << device.id << ": (" << device.GetFlowString() << ") " << device.name << '\n';
}

TEST_F(ScmAudioTests, DISABLED_LoopbackCapture)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);

    auto deviceListResult = engine.ListAudioDevices();
    FAIL_ON_ERROR(deviceListResult);
    const auto deviceList = deviceListResult.GetValue();

    auto setInputResult = engine.SetInputDevice(deviceList[8]);
    FAIL_ON_ERROR(setInputResult);
    auto setOutputResult = engine.SetOutputDevice(deviceList[3]);
    FAIL_ON_ERROR(setOutputResult);

    auto igniteResult = engine.Ignite();
    FAIL_ON_ERROR(igniteResult);

    char input;
    std::cout << "\nRunning ... press <enter> to quit.\n";
    std::cin.get(input);
}

TEST_F(ScmAudioTests, PlayMp3)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);

    auto setDefaultResult = engine.SetDefaultDevices();
    FAIL_ON_ERROR(setDefaultResult);

    engine.SetCapture(false);

    Result<SoundId> loadSoundResult = engine.LoadSound(R"(D:\Music\Limp Bizkit - Greatest Hitz\Limp Bizkit - Faith.mp3)");
    FAIL_ON_ERROR(loadSoundResult);
    const SoundId soundId = loadSoundResult.GetValue();

    auto igniteResult = engine.Ignite();
    FAIL_ON_ERROR(igniteResult);

    engine.Play(soundId);

    char input;
    std::cout << "\nRunning ... press <enter> to quit.\n";
    std::cin.get(input);
}

} // namespace MyCode