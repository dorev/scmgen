cmake_minimum_required(VERSION 3.14)
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
add_subdirectory(${DEPENDENCIES_DIRECTORY}/fftw)

# ==============================================================================
# LIBNYQUIST
# ==============================================================================
set(LIBNYQUIST_BUILD_EXAMPLE OFF CACHE BOOL "Skip generating Libnyquist examples")
add_subdirectory(${DEPENDENCIES_DIRECTORY}/libnyquist)
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
add_subdirectory(${DEPENDENCIES_DIRECTORY}/rtaudio)

# ==============================================================================
# SCMAUDIO
# ==============================================================================
add_subdirectory(${SOLUTION_ROOT}/scmaudio) 