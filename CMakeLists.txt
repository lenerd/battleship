cmake_minimum_required(VERSION 3.0)
project(Battleship CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(BUILD_SHARED_LIBS "Build libraries as SHARED" OFF)

set(Battleship_INSTALL_PREFIX "${Battleship_BINARY_DIR}/prefix")
set(Battleship_DEFAULT_ARGS
    "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}"
    "-DCMAKE_PREFIX_PATH:PATH=${Battleship_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}"
    "-DCMAKE_INSTALL_PREFIX:PATH=${Battleship_INSTALL_PREFIX}"
)
file(MAKE_DIRECTORY ${Battleship_INSTALL_PREFIX})

# build external dependencies
add_subdirectory(extern)

include(ExternalProject)
ExternalProject_Add(battleship
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/src
    CMAKE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}
        -DDEPENDENCY_PREFIX:PATH=${Battleship_INSTALL_PREFIX}
        -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
)
add_dependencies(battleship
    flatbuffers
    nlohmann_json
    Beast
    Crypto++
    ABY
)
