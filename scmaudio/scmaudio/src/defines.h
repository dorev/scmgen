#pragma once

// Export macros
#if defined(SCMAUDIO_DLL) && defined(_WIN32)
    #ifdef SCMAUDIO_EXPORT
        #define SCMAUDIO_API __declspec(dllexport)
    #else
        #define SCMAUDIO_API __declspec(dllimport)
    #endif
#else
    #define SCMAUDIO_API
#endif

// Utilities
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

#define _2PI 6.28318530718f
#define _PI 3.14159265359f
#define CASE_RETURN_STRING(enum, value) case enum::value: return #value
#define BIT(x) (1 << x)
