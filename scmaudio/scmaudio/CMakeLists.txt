InitCpp17Project(ScmAudio)
GatherSources(SOURCES)
SetFolderHierarchy(${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES})

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
)

set(DEBUG_OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY}/Debug)
set(RELEASE_OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY}/Release)

SET(LIBRARIES_TO_LINK
    debug ${DEBUG_OUTPUT_DIRECTORY}/fftw3f.lib
    debug ${DEBUG_OUTPUT_DIRECTORY}/libnyquist_d.lib
    debug ${DEBUG_OUTPUT_DIRECTORY}/rtaudiod.lib
    optimized ${RELEASE_OUTPUT_DIRECTORY}/fftw3f.lib
    optimized ${RELEASE_OUTPUT_DIRECTORY}/libnyquist.lib
    optimized ${RELEASE_OUTPUT_DIRECTORY}/rtaudio.lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES_TO_LINK})
add_dependencies(ScmAudio fftw3f libnyquist rtaudio)