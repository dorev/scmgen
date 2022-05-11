#pragma once

#include "defines.h"
#include "types.h"

namespace ScmAudio
{

class ErrorCode
{
public:
    static constexpr U64 ScmAudioErrors = 64ULL << 32;
    static constexpr U64 DeviceNotSupported = ScmAudioErrors | 0;
    static constexpr U64 RtAudioNotInitialized = ScmAudioErrors | 1;
    static constexpr U64 AudioEngineInError = ScmAudioErrors | 2;
    static constexpr U64 UnableToProbeDevice = ScmAudioErrors | 3;
    static constexpr U64 NoValidDeviceAvailable = ScmAudioErrors | 4;
    static constexpr U64 UnsupportedInputSamplingRate = ScmAudioErrors | 5;
    static constexpr U64 UnsupportedOutputSamplingRate = ScmAudioErrors | 6;
    static constexpr U64 UnsupportedBufferSize = ScmAudioErrors | 7;
    static constexpr U64 EngineAlreadyRunning = ScmAudioErrors | 8;
    static constexpr U64 SoundAlreadyLoaded = ScmAudioErrors | 9;
    static constexpr U64 FileNotSupported = ScmAudioErrors | 10;
    static constexpr U64 UnableToLoadSound = ScmAudioErrors | 11;
    static constexpr U64 UnableToStoreSound = ScmAudioErrors | 12;
    static constexpr U64 SoundNotAvailable = ScmAudioErrors | 13;

    static constexpr U64 RtAudioErrors = 65ULL << 32;
    static constexpr U64 WARNING = RtAudioErrors | 0;
    static constexpr U64 UNKNOWN_ERROR = RtAudioErrors | 1;
    static constexpr U64 NO_DEVICES_FOUND = RtAudioErrors | 2;
    static constexpr U64 INVALID_DEVICE = RtAudioErrors | 3;
    static constexpr U64 DEVICE_DISCONNECT = RtAudioErrors | 4;
    static constexpr U64 MEMORY_ERROR = RtAudioErrors | 5;
    static constexpr U64 INVALID_PARAMETER = RtAudioErrors | 6;
    static constexpr U64 INVALID_USE = RtAudioErrors | 7;
    static constexpr U64 DRIVER_ERROR = RtAudioErrors | 8;
    static constexpr U64 SYSTEM_ERROR = RtAudioErrors | 9;
    static constexpr U64 THREAD_ERROR = RtAudioErrors | 10;

    static constexpr U64 GenericCategory = 0ULL << 32;
    static constexpr U64 NoError = GenericCategory | 0;
    static constexpr U64 Undefined = GenericCategory | 1;
    static constexpr U64 InvalidParameter = GenericCategory | 2;

    static constexpr U64 ResultCategory = 1ULL << 32;
    static constexpr U64 UninitializedError = ResultCategory | 0;
    static constexpr U64 AsyncResultLocked = ResultCategory | 1;

    static constexpr U64 TaskCategory = 2ULL << 32;
    static constexpr U64 NoTaskToExecute = TaskCategory | 0;
    static constexpr U64 TaskManagerAlreadyStarted = TaskCategory | 1;
    static constexpr U64 TaskCancelled = TaskCategory | 2;

    static constexpr U64 CsvCategory = 3ULL << 32;
    static constexpr U64 ColumnCountMismatch = CsvCategory | 0;

    static constexpr U64 FileCategory = 4ULL << 32;
    static constexpr U64 UnableToCreateFile = FileCategory | 0;
    static constexpr U64 UnableToOpenFile = FileCategory | 1;
    static constexpr U64 FileDoesNotExist = FileCategory | 2;
    static constexpr U64 UnableToGetFileHandle = FileCategory | 3;


};


///////////////////////////////////////////////////////////////////////////////
// Error
///////////////////////////////////////////////////////////////////////////////

#define MAKE_ERROR(errorName) Error(ErrorCode::##errorName, #errorName)

class Error
{
public:

    constexpr Error()
        : _code(ErrorCode::UninitializedError)
        , _description("Uninitialized Error")
    {
    }

    constexpr Error(U64 code, const char* description)
        : _code(code)
        , _description(description)
    {
    }

    constexpr Error(U64 code)
        : _code(code)
        , _description(nullptr)
    {
    }

    constexpr bool operator==(const Error& other) const
    {
        if(_code != other._code)
            return false;
        if(_description == nullptr)
            return other._description == nullptr;
        if(other._description == nullptr)
            return false;
        return strcmp(_description, other._description) == 0;
    }

    constexpr U64 GetCode() const
    {
        return _code;
    }

    constexpr const char* GetDescription() const
    {
        return _description;
    }

    constexpr explicit operator bool() const
    {
        return _code != ErrorCode::NoError;
    }

private:

    const char* _description;
    U64 _code;
};

} // namespace ScmAudio