#include "gtest/gtest.h"
#include "audioengine.h"

namespace ScmAudio
{

class ScmAudioTests : public ::testing::Test
{
};

TEST_F(ScmAudioTests, ListDevices)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);

    auto deviceListResult = engine.ListAudioDevices();
    if (deviceListResult.HasError())
        FAIL() << deviceListResult.GetError().GetDescription();

    const auto deviceList = deviceListResult.GetValue();
    for (const auto& device : deviceList)
        std::cout << device.id << ": (" << device.GetFlowString() << ") " << device.name << '\n';
}

TEST_F(ScmAudioTests, PlaySine)
{
}

TEST_F(ScmAudioTests, LoopbackCapture)
{
}

TEST_F(ScmAudioTests, PlayMp3)
{
}

} // namespace MyCode