InitCppProject(17 Tests)

add_executable(${PROJECT_NAME}
    main.cpp
    fftprecisiontest.cpp
    noteprecisiontest.cpp
    audioenginetests.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SCMAUDIO_DIRECTORY}/src
    gtest
    gtest/include
    gmock
    gmock/include
)

target_link_libraries(${PROJECT_NAME} ScmAudio)