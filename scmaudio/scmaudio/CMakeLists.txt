InitCppProject(17 ScmAudio)
GatherSources(SOURCES "src")
SetFolderHierarchy(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SCMAUDIO_DLL
    SCMAUDIO_EXPORT
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${RTAUDIO_DIRECTORY}
    ${FFTW_DIRECTORY}/api
)

set(DEBUG_OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY}/Debug)
set(RELEASE_OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY}/Release)
target_link_libraries(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${DEBUG_OUTPUT_DIRECTORY}/fftw3f.lib>
    $<$<CONFIG:Debug>:${DEBUG_OUTPUT_DIRECTORY}/libnyquist_d.lib>
    $<$<CONFIG:Debug>:${DEBUG_OUTPUT_DIRECTORY}/rtaudiod.lib>
    $<$<CONFIG:Release>:${RELEASE_OUTPUT_DIRECTORY}/fftw3f.lib>
    $<$<CONFIG:Release>:${RELEASE_OUTPUT_DIRECTORY}/libnyquist.lib>
    $<$<CONFIG:Release>:${RELEASE_OUTPUT_DIRECTORY}/rtaudio.lib>
)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

add_dependencies(ScmAudio fftw3f libnyquist rtaudio)