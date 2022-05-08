# https://preshing.com/20170522/learn-cmakes-scripting-language-in-15-minutes/

macro (InitCppProject cppStandard projectName)
    set(CMAKE_CXX_STANDARD ${cppStandard})
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    project(${projectName} LANGUAGES CXX)
endmacro(InitCppProject)

# ==============================================================================
macro (GatherSources outputVariable)
    # Treat first extra argument as the folder to explore
    # otherwise default to CMAKE_CURRENT_SOURCE_DIR
    set(argv ${ARGN})
    list(LENGTH argv argc)
    if(argc GREATER_EQUAL 1)
        list(GET argv 0 rootFolder)
    else()
        set(rootFolder ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    file(GLOB_RECURSE
        ${outputVariable}
        ${rootFolder}/*.c
        ${rootFolder}/*.cpp
        ${rootFolder}/*.cxx
        ${rootFolder}/*.h
        ${rootFolder}/*.hpp
    )
endmacro(GatherSources)

# ==============================================================================

function(SetFolderHierarchy rootFolder sourceList)
    foreach(sourceFile IN ITEMS ${${sourceList}})
        if (IS_ABSOLUTE "${sourceFile}")
            file(RELATIVE_PATH sourceRelativePath "${rootFolder}" "${sourceFile}")
        else()
            set(sourceRelativePath "${sourceFile}")
        endif()
        get_filename_component(sourceFolderPath "${sourceRelativePath}" DIRECTORY)
        string(REPLACE "/" "\\" sourcePathMsvc "${sourceFolderPath}")
        source_group("${sourcePathMsvc}" FILES "${sourceFile}")
    endforeach()
endfunction(SetFolderHierarchy)

# ==============================================================================

macro (SwigCsharp swigFile outputDirectory namespace)

    if(NOT DEFINED SWIG_EXE)
        message( FATAL_ERROR "SWIG_EXE not defined")
    endif()

    set(SWIG_COMMAND "${SWIG_EXE} -c++ -csharp ${outputDirectory}")

    if("${namespace}" NOT STREQUAL "")
        message("Namespace defined! : ${namespace}")
        set(SWIG_COMMAND "${SWIG_COMMAND} -namespace ${namespace}")
    endif()

    set(SWIG_COMMAND "${SWIG_COMMAND} ${swigFile}")
# D:\SWIG\swigwin-4.0.2\swig.exe -c++ -csharp -namespace ScmAudio -outdir outputDirectory scmaudio.swig"
endmacro()