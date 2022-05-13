#pragma once

#include "types.h"
#include "defines.h"

namespace ScmAudio
{

inline void GenerateSinWave(F32* buffer, U32 bufferSize, F32 frequency, U32 samplingRate)
{
    F32 sampleDurationSeconds = 1.0f / samplingRate;
    for (U32 i = 0; i < bufferSize; ++i)
    {
        F32 sampleTime = i * sampleDurationSeconds;
        buffer[i] = sin(_2PI * frequency * sampleTime);
    }
}

inline F32 FrequencyOfNote(U32 note, F32 a4Frequency)
{
    static const F32 a = pow(2.0f, (1.0f / 12.0f));
    F32 delta = static_cast<F32>(note) - 57.0f; // A4
    return a4Frequency * pow(a, delta);
}

static const U32 Sharp = 0;
static const U32 Flat = 1;

static const char* NoteNames[12][2] =
{
    { "C", "C" },
    { "C#", "Db" },
    { "D", "D" },
    { "D#", "Eb" },
    { "E", "E" },
    { "F", "F" },
    { "F#", "Gb" },
    { "G", "G" },
    { "G#", "Ab" },
    { "A", "A" },
    { "A#", "Bb" },
    { "B", "B" }
};

static constexpr U8 C = 0;
static constexpr U8 Cs = 1;
static constexpr U8 Df = 1;
static constexpr U8 D = 2;
static constexpr U8 Ds = 3;
static constexpr U8 Ef = 3;
static constexpr U8 E = 4;
static constexpr U8 F = 5;
static constexpr U8 Fs = 6;
static constexpr U8 Gf = 6;
static constexpr U8 G = 7;
static constexpr U8 Gs = 8;
static constexpr U8 Af = 8;
static constexpr U8 A = 9;
static constexpr U8 As = 10;
static constexpr U8 Bf = 10;
static constexpr U8 B = 11;

static constexpr U8 M7[3] = { 4, 7, 10 };
static constexpr U8 MM7[3] = { 4, 7, 11 };
static constexpr U8 m7[3] = { 3, 7, 10 };
static constexpr U8 mM7[3] = { 3, 7, 11 };
static constexpr U8 Diminished[3] = { 3, 6, 9 };
static constexpr U8 Augmented[3] = { 4, 8, 0 };

static constexpr U8 octaveLimit = 11;

static constexpr float C_Hz[octaveLimit] = { 16.3515978312874f, 32.70319566f, 65.40639133f, 130.8127827f, 261.6255653f, 523.2511306f, 1046.502261f, 2093.004522f, 4186.009045f, 8372.01809f, 16744.03618f };
static constexpr float Cs_Hz[octaveLimit] = { 17.3239144360545f, 34.64782887f, 69.29565774f, 138.5913155f, 277.182631f, 554.365262f, 1108.730524f, 2217.461048f, 4434.922096f, 8869.844191f, 17739.68838f };
static constexpr float Df_Hz[octaveLimit] = { 17.3239144360545f, 34.64782887f, 69.29565774f, 138.5913155f, 277.182631f, 554.365262f, 1108.730524f, 2217.461048f, 4434.922096f, 8869.844191f, 17739.68838f };
static constexpr float D_Hz[octaveLimit] = { 18.354047994838f, 36.70809599f, 73.41619198f, 146.832384f, 293.6647679f, 587.3295358f, 1174.659072f, 2349.318143f, 4698.636287f, 9397.272573f, 18794.54515f };
static constexpr float Ds_Hz[octaveLimit] = { 19.4454364826301f, 38.89087297f, 77.78174593f, 155.5634919f, 311.1269837f, 622.2539674f, 1244.507935f, 2489.01587f, 4978.03174f, 9956.063479f, 19912.12696f };
static constexpr float Ef_Hz[octaveLimit] = { 19.4454364826301f, 38.89087297f, 77.78174593f, 155.5634919f, 311.1269837f, 622.2539674f, 1244.507935f, 2489.01587f, 4978.03174f, 9956.063479f, 19912.12696f };
static constexpr float E_Hz[octaveLimit] = { 20.6017223070544f, 41.20344461f, 82.40688923f, 164.8137785f, 329.6275569f, 659.2551138f, 1318.510228f, 2637.020455f, 5274.040911f, 10548.08182f, 21096.16364f };
static constexpr float F_Hz[octaveLimit] = { 21.8267644645627f, 43.65352893f, 87.30705786f, 174.6141157f, 349.2282314f, 698.4564629f, 1396.912926f, 2793.825851f, 5587.651703f, 11175.30341f, 22350.60681f };
static constexpr float Fs_Hz[octaveLimit] = { 23.1246514194772f, 46.24930284f, 92.49860568f, 184.9972114f, 369.9944227f, 739.9888454f, 1479.977691f, 2959.955382f, 5919.910763f, 11839.82153f, 23679.64305f };
static constexpr float Gf_Hz[octaveLimit] = { 23.1246514194772f, 46.24930284f, 92.49860568f, 184.9972114f, 369.9944227f, 739.9888454f, 1479.977691f, 2959.955382f, 5919.910763f, 11839.82153f, 23679.64305f };
static constexpr float G_Hz[octaveLimit] = { 24.4997147488593f, 48.9994295f, 97.998859f, 195.997718f, 391.995436f, 783.990872f, 1567.981744f, 3135.963488f, 6271.926976f, 12543.85395f, 25087.7079f };
static constexpr float Gs_Hz[octaveLimit] = { 25.9565436f, 51.9130872f, 103.8261744f, 207.6523488f, 415.3046976f, 830.6093952f, 1661.21879f, 3322.437581f, 6644.875161f, 13289.75032f, 26579.50065f };
static constexpr float Af_Hz[octaveLimit] = { 25.9565436f, 51.9130872f, 103.8261744f, 207.6523488f, 415.3046976f, 830.6093952f, 1661.21879f, 3322.437581f, 6644.875161f, 13289.75032f, 26579.50065f };
static constexpr float A_Hz[octaveLimit] = { 27.5f, 55.0f, 110.0f, 220.0f, 440.0f, 880.0f, 1760.0f, 3520.0f, 7040.0f, 14080.0f, 28160.0f };
static constexpr float As_Hz[octaveLimit] = { 29.13523509f, 58.27047019f, 116.5409404f, 233.0818808f, 466.1637615f, 932.327523f, 1864.655046f, 3729.310092f, 7458.620184f, 14917.24037f, 29834.48074f };
static constexpr float Bf_Hz[octaveLimit] = { 29.13523509f, 58.27047019f, 116.5409404f, 233.0818808f, 466.1637615f, 932.327523f, 1864.655046f, 3729.310092f, 7458.620184f, 14917.24037f, 29834.48074f };
static constexpr float B_Hz[octaveLimit] = { 30.86770633f, 61.73541266f, 123.4708253f, 246.9416506f, 493.8833013f, 987.7666025f, 1975.533205f, 3951.06641f, 7902.13282f, 15804.26564f, 31608.53128f };

static constexpr U8 C_Midi[octaveLimit] = { 0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120 };
static constexpr U8 Cs_Midi[octaveLimit] = { 1, 13, 25, 37, 49, 61, 73, 85, 97, 109, 121 };
static constexpr U8 Df_Midi[octaveLimit] = { 1, 13, 25, 37, 49, 61, 73, 85, 97, 109, 121 };
static constexpr U8 D_Midi[octaveLimit] = { 2, 14, 26, 38, 50, 62, 74, 86, 98, 110, 122 };
static constexpr U8 Ds_Midi[octaveLimit] = { 3, 15, 27, 39, 51, 63, 75, 87, 99, 111, 123 };
static constexpr U8 Ef_Midi[octaveLimit] = { 3, 15, 27, 39, 51, 63, 75, 87, 99, 111, 123 };
static constexpr U8 E_Midi[octaveLimit] = { 4, 16, 28, 40, 52, 64, 76, 88, 100, 112, 124 };
static constexpr U8 F_Midi[octaveLimit] = { 5, 17, 29, 41, 53, 65, 77, 89, 101, 113, 125 };
static constexpr U8 Fs_Midi[octaveLimit] = { 6, 18, 30, 42, 54, 66, 78, 90, 102, 114, 126 };
static constexpr U8 Gf_Midi[octaveLimit] = { 6, 18, 30, 42, 54, 66, 78, 90, 102, 114, 126 };
static constexpr U8 G_Midi[octaveLimit] = { 7, 19, 31, 43, 55, 67, 79, 91, 103, 115, 127 };
static constexpr U8 Gs_Midi[octaveLimit] = { 8, 20, 32, 44, 56, 68, 80, 92, 104, 116, 128 };
static constexpr U8 Af_Midi[octaveLimit] = { 8, 20, 32, 44, 56, 68, 80, 92, 104, 116, 128 };
static constexpr U8 A_Midi[octaveLimit] = { 9, 21, 33, 45, 57, 69, 81, 93, 105, 117, 129 };
static constexpr U8 As_Midi[octaveLimit] = { 10, 22, 34, 46, 58, 70, 82, 94, 106, 118, 130 };
static constexpr U8 Bf_Midi[octaveLimit] = { 10, 22, 34, 46, 58, 70, 82, 94, 106, 118, 130 };
static constexpr U8 B_Midi[octaveLimit] = { 11, 23, 35, 47, 59, 71, 83, 95, 107, 119, 131 };

} // namespace ScmAudio
