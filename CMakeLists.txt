cmake_minimum_required(VERSION 3.0.0)

# set the project name
project(data_structures_and_algorithms VERSION 0.1.0)

set(
    SOURCE_FILES
    main.cpp
)

set(EXE_FILE poco-logger)

set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# add_library(lins355_static STATIC ${SOURCE_FILES})
include(CTest)
enable_testing()

# add the executable
add_executable(
    ${EXE_FILE}

    ${SOURCE_FILES}
)

# include dir
# include_directories(
# libserial
# libserial/src
# )

# Directories of 3rd lib
# link_directories(
# libserial/build/lib
# )
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# specify the C++ standard
# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set debug flag
set(CMAKE_BUILD_TYPE Debug)

# add_subdirectory(
# unit_test
# )

# Link neccessary libraries, include 3rd libraries
target_link_libraries(
    ${EXE_FILE}

    PocoFoundation
    PocoUtil
    PocoXML
    PocoJSON
    pthread
)