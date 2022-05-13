#pragma once

#ifdef SCMAUDIO_DLL
    #ifdef SCMAUDIO_EXPORT
        #define SCMAUDIO_API __declspec(dllexport)
    #else
        #define SCMAUDIO_API __declspec(dllimport)
    #endif
#else
    #define SCMAUDIO_API
#endif

#define DELETE_COPY_CONSTRUCTOR(Class) \
    Class(const Class&) = delete; \
    Class(Class&&) = delete; \
    Class& operator=(const Class&) = delete; \
    Class& operator=(Class&&) = delete;

#define DELETE_DEFAULT_CONSTRUCTOR(Class) \
    Class() = delete; \
    Class(const Class&) = delete; \
    Class(Class&&) = delete;

#define UNUSED(variable) (void)variable

#define BREAK(message) \
{ \
    std::cout << "\n\n" << message << "\nin  --> " << __FILE__ << "@ l." << __LINE__ << "\n\n"; \
    *(reinterpret_cast<unsigned int*>(0xBAAAAADBADBADBAD)) = 0U; \
    throw; \
}

#define ASSERT(condition) \
if(!(condition)) \
{ \
    BREAK("Assertion failed: " #condition); \
}
