cmake_minimum_required(VERSION 3.15)
include(utils.cmake)

# ==============================================================================
# Solution configuration
# ==============================================================================

project(ScmAudioSolution)
set(SOLUTION_ROOT ${CMAKE_SOURCE_DIR})
set(SOLUTION_OUTPUT_DIRECTORY ${SOLUTION_ROOT}/bin)
set(DEPENDENCIES_DIRECTORY ${SOLUTION_ROOT}/dependencies)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY})
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# ==============================================================================
# GENERIC CMAKE OPTION
# ==============================================================================
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries")

# ==============================================================================
# FFTW 
# ==============================================================================
set(ENABLE_FLOAT ON CACHE BOOL "Set FFTW primitive to float")
set(BUILD_TESTS OFF CACHE BOOL "Skip generating FFTW tests")
set(FFTW_DIRECTORY ${DEPENDENCIES_DIRECTORY}/fftw)
add_subdirectory(${FFTW_DIRECTORY})

# ==============================================================================
# LIBNYQUIST
# ==============================================================================
set(LIBNYQUIST_BUILD_EXAMPLE OFF CACHE BOOL "Skip generating Libnyquist examples")
set(LIBNYQUIST_DIRECTORY ${DEPENDENCIES_DIRECTORY}/libnyquist)
add_subdirectory(${LIBNYQUIST_DIRECTORY})
set_target_properties(libnyquist
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${SOLUTION_OUTPUT_DIRECTORY}"
    ARCHIVE_OUTPUT_DIRECTORY "${SOLUTION_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY "${SOLUTION_OUTPUT_DIRECTORY}"
)

# ==============================================================================
# RTAUDIO
# ==============================================================================
set(RTAUDIO_BUILD_TESTING OFF CACHE BOOL "Skip generating RtAudio tests")
set(RTAUDIO_DIRECTORY ${DEPENDENCIES_DIRECTORY}/rtaudio)
add_subdirectory(${RTAUDIO_DIRECTORY})

# ==============================================================================
# MOODYCAMEL READERWRITERQUEUE
# ==============================================================================
set(READERWRITERQUEUE_DIRECTORY ${DEPENDENCIES_DIRECTORY}/readerwriterqueue)
add_subdirectory(${READERWRITERQUEUE_DIRECTORY})

# ==============================================================================
# SCMAUDIO
# ==============================================================================
set(SCMAUDIO_DIRECTORY ${SOLUTION_ROOT}/scmaudio)
add_subdirectory(${SCMAUDIO_DIRECTORY})

# ==============================================================================
# TESTS
# ==============================================================================
add_subdirectory(tests)