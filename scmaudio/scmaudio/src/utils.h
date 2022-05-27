#pragma once

#include <cmath>
#include <type_traits>
#include <thread>
#include <chrono>

namespace ScmAudio
{

constexpr U32 InvalidId = U32(-1);

template <class T, class U>
T* ToPtr(U data)
{
    if constexpr (std::is_pointer<U>::value)
        return reinterpret_cast<T*>(data);
    else
        return reinterpret_cast<T*>(&data);
}

template <class T, class U>
bool Contains(const T& container, const U& value)
{
    return std::find(container.begin(), container.end(), value) != container.end();
}

template <class T>
U32 Hash(const T& object)
{
    static std::hash<T> hasher;
    return static_cast<U32>(hasher(object));
}

template <class T, class U>
F32 DifferencePercentage(T value1, U value2)
{
    return abs(F32(value1) - F32(value2)) / ((F32(value1) + F32(value2)) / 2.0f) * 100.0f;
}

inline void Sleep(U32 milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

inline U64 TimeNow()
{
    using namespace std::chrono;
    return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count();
}

} // namespace ScmAudio
