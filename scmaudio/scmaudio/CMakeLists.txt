InitCppProject(17 ScmAudio)
GatherSources(SOURCES "src")
SetFolderHierarchy(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    SCMAUDIO_DLL
    SCMAUDIO_EXPORT
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_DIRECTORY}
    ${LIBNYQUIST_DIRECTORY}/include
    ${FFTW_DIRECTORY}/api
)

set(OUTPUT_DIRECTORY ${SOLUTION_OUTPUT_DIRECTORY})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    readerwriterqueue
    rtaudio
    libnyquist
    fftw3f
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

add_dependencies(ScmAudio fftw3f libnyquist rtaudio readerwriterqueue)