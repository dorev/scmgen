cmake_minimum_required(VERSION 3.14)

project(ScmAudioSolution)

set(SCMAUDIO_ROOT ${CMAKE_SOURCE_DIR})
set(BINARY_OUTPUT_FOLDER ${SCMAUDIO_ROOT}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUTPUT_FOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUTPUT_FOLDER})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_FOLDER})
set(DEPENDECIES_DIR ${SCMAUDIO_ROOT}/dependencies)

# Global options
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) # prevents warnings from the following lines
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries")

# FFTW
set(ENABLE_FLOAT ON CACHE BOOL "Set FFTW primitive to float")
set(BUILD_TESTS OFF CACHE BOOL "Skip generating FFTW tests")
add_subdirectory(${DEPENDECIES_DIR}/fftw)

# LIBNYQUIST
set(LIBNYQUIST_BUILD_EXAMPLE OFF CACHE BOOL "Skip generating Libnyquist examples")
add_subdirectory(${DEPENDECIES_DIR}/libnyquist)
set_target_properties(libnyquist
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${BINARY_OUTPUT_FOLDER}"
    ARCHIVE_OUTPUT_DIRECTORY "${BINARY_OUTPUT_FOLDER}"
    RUNTIME_OUTPUT_DIRECTORY "${BINARY_OUTPUT_FOLDER}"
)

# RTAUDIO
set(RTAUDIO_BUILD_TESTING OFF CACHE BOOL "Skip generating RtAudio tests")
add_subdirectory(${DEPENDECIES_DIR}/rtaudio)

# SCMAUDIO
#add_subdirectory(${SCMAUDIO_ROOT}/scmaudio)

# https://stackoverflow.com/questions/27570567/is-it-possible-to-set-cmake-variables-with-add-subdirectory/27573961