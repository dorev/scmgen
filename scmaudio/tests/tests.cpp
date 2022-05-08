#include "gtest/gtest.h"
#include "audioengine.h"

namespace ScmAudio
{

class Tests : public ::testing::Test
{
protected:

    Tests()
    {
    }

    ~Tests() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};


TEST_F(Tests, ListDevices)
{
    AudioEngine engine;
    EXPECT_EQ(engine.GetStatus(), AudioEngine::Stopped);

    auto deviceListResult = engine.ListAudioDevices();
    if (deviceListResult.HasError())
        FAIL() << deviceListResult.GetError().GetDescription();

    const auto deviceList = deviceListResult.GetValue();

    for (const auto& device : deviceList)
        std::cout << device.id << ": " << device.name << " is " << device.GetFlowString() << '\n';
}

} // namespace MyCode