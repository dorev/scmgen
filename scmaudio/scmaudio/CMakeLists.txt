cmake_minimum_required(VERSION 3.14)

project(ScmAudio)

file(GLOB_RECURSE
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    ${BINARY_OUTPUT_FOLDER}/fftw3f.lib
    ${BINARY_OUTPUT_FOLDER}/libnyquist.lib
    ${BINARY_OUTPUT_FOLDER}/rtaudio.lib
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)