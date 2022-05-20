#include "gtest/gtest.h"
#include "audioengine.h"

namespace ScmAudio
{

class AudioEngineTests : public ::testing::Test
{
};

// Helper macros
#define FAIL_ON_ERROR(result) if(result.HasError()) { FAIL() << result.GetError().GetDescription(); }
#define TEST_MP3_PATH R"(../../tests/Kissing The Shadows.mp3)"
#define PRESS_ENTER_TO_QUIT \
char input; \
std::cout << "\nRunning ... press <enter> to quit.\n"; \
std::cin.get(input);

// Helper variables & methods
U32 gOutputDevice = U32(-1);
U32 gInputDevice = U32(-1);
U32 gDeviceCount = U32(0);
bool DeviceIdIsValid(U32 device) { return device < gDeviceCount; }

void SelectDevice(AudioEngine& engine)
{
    auto deviceListResult = engine.ListAudioDevices();
    FAIL_ON_ERROR(deviceListResult);

    const auto deviceList = deviceListResult.GetValue();

    auto setInputResult = engine.SetInputDevice(deviceList[gInputDevice]);
    FAIL_ON_ERROR(setInputResult);
    auto setOutputResult = engine.SetOutputDevice(deviceList[gOutputDevice]);
    FAIL_ON_ERROR(setOutputResult);
}

TEST_F(AudioEngineTests, ListDevices)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);

    auto deviceListResult = engine.ListAudioDevices();
    FAIL_ON_ERROR(deviceListResult);

    const auto deviceList = deviceListResult.GetValue();
    gDeviceCount = U32(deviceList.size());
    for (const auto& device : deviceList)
        std::cout << device.id << ": (" << device.GetFlowString() << ") " << device.name << '\n';
}

TEST_F(AudioEngineTests, SelectDevicesForTests)
{
    while (!DeviceIdIsValid(gOutputDevice))
    {
        std::cout << "Select OUTPUT device for tests : ";
        std::cin >> gOutputDevice;
    }
    while (!DeviceIdIsValid(gInputDevice))
    {
        std::cout << "Select INPUT device for tests : ";
        std::cin >> gInputDevice;
    }
    std::cin.ignore();
    std::cout << '\n';
}

TEST_F(AudioEngineTests, LoopbackCapture)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);
    SelectDevice(engine);

    auto igniteResult = engine.Ignite();
    FAIL_ON_ERROR(igniteResult);

    PRESS_ENTER_TO_QUIT;
}

TEST_F(AudioEngineTests, PlayMp3)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);
    SelectDevice(engine);

    engine.SetCapture(false);

    Result<SoundId> loadSoundResult = engine.LoadSound(TEST_MP3_PATH);
    FAIL_ON_ERROR(loadSoundResult);
    const SoundId soundId = loadSoundResult.GetValue();

    auto igniteResult = engine.Ignite();
    FAIL_ON_ERROR(igniteResult);

    engine.Play(soundId);

    PRESS_ENTER_TO_QUIT;
}

TEST_F(AudioEngineTests, PlayMp3Twice)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);
    SelectDevice(engine);

    engine.SetCapture(false);

    Result<SoundId> loadSoundResult = engine.LoadSound(TEST_MP3_PATH);
    FAIL_ON_ERROR(loadSoundResult);
    const SoundId soundId = loadSoundResult.GetValue();

    auto igniteResult = engine.Ignite();
    FAIL_ON_ERROR(igniteResult);

    engine.Play(soundId);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    engine.Play(soundId);

    PRESS_ENTER_TO_QUIT;
}

TEST_F(AudioEngineTests, PlayMp3MixedWithCapture)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);
    SelectDevice(engine);

    engine.SetCapture(true);

    Result<SoundId> loadSoundResult = engine.LoadSound(TEST_MP3_PATH);
    FAIL_ON_ERROR(loadSoundResult);
    const SoundId soundId = loadSoundResult.GetValue();

    auto igniteResult = engine.Ignite();
    FAIL_ON_ERROR(igniteResult);

    engine.Play(soundId);

    PRESS_ENTER_TO_QUIT;
}

} // namespace MyCode