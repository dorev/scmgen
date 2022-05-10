#pragma once

#include <type_traits>

namespace ScmAudio
{
    template <class T>
    void* ToVoidPtr(T object)
    {
        if constexpr (std::is_pointer<T>::value)
            return static_cast<void*>(object);
        else
            return static_cast<void*>(&object);
    }

    template <class T, class U>
    bool Contains(const T& container, const U& value)
    {
        return std::find(container.begin(), container.end(), value) != container.end();
    }

} // namespace ScmAudio