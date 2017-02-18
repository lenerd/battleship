cmake_minimum_required(VERSION 3.0)
project(Battleship CXX)

set(CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/../cmake"
    "${PROJECT_SOURCE_DIR}/../extern/flatbuffers/CMake"
)

set(CMAKE_INCLUDE_PATH ${DEPENDENCY_PREFIX}/include)
set(CMAKE_LIBRARY_PATH ${DEPENDENCY_PREFIX}/lib ${DEPENDENCY_PREFIX}/lib64)

find_package(Boost REQUIRED COMPONENTS program_options system)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest)
find_library(CRYPTO++_LIBRARY libcryptopp.a)
if ((NOT CRYPTO++_LIBRARY) OR (NOT EXISTS ${CRYPTO++_LIBRARY}))
    message(FATAL_ERROR "Crypto++ not found")
else()
    message(STATUS "Found Crypto++: ${CRYPTO++_LIBRARY}")
endif()


set(BATTLESHIP_SOURCES
    crypto/committer.cpp
    crypto/hash_committer.cpp
    crypto/naor_committer.cpp
    game/board.cpp
    game/game_communicator.cpp
    game/game.cpp
    misc/options.cpp
    misc/util.cpp
    network/dummy_connection.cpp
    network/tcp_connection.cpp
    ui/board_window.cpp
    ui/game_screen.cpp
    ui/message_window.cpp
    ui/ncurses_interface.cpp
    ui/ncurses_window.cpp
    ui/user_interface.cpp
    ui/web_interface.cpp
    ui/websocket_connection.cpp
)

set(INCLUDES
    ${CMAKE_CURRENT_BINARY_DIR}/generated_includes
    ${Boost_INCLUDE_DIR} ${CMAKE_INCLUDE_PATH}
)

set(LIBRARIES
    Boost::program_options Boost::system
    Threads::Threads
    ${CURSES_LIBRARIES}
    ${CRYPTO++_LIBRARY}
)

# generating flatbuffers header files
file(GLOB_RECURSE FBS_SCHEMAS ${CMAKE_CURRENT_SOURCE_DIR} "*.fbs")
set(FLATBUFFERS_FLATC_SCHEMA_EXTRA_ARGS --scoped-enums)
set(FLATBUFFERS_FLATC_EXECUTABLE ${DEPENDENCY_PREFIX}/bin/flatc)
include(BuildFlatBuffers)
build_flatbuffers("${FBS_SCHEMAS}" "" flatbuffers_header ""
    "${CMAKE_CURRENT_BINARY_DIR}/generated_includes" "" "")


add_library(battleship ${BATTLESHIP_SOURCES})
add_dependencies(battleship flatbuffers_header)
target_include_directories(battleship PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(battleship SYSTEM PRIVATE ${INCLUDES})

add_executable(battleship.exe game/battleship.cpp)
target_include_directories(battleship.exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(battleship.exe SYSTEM PRIVATE ${INCLUDES})
target_link_libraries(battleship.exe battleship ${LIBRARIES})

set(TEST_SOURCES
    test/test.cpp
    test/test_board.cpp
    test/test_dummy_connection.cpp
    test/test_game_communicator.cpp
    test/test_committer.cpp
    test/test_hash_committer.cpp
    test/test_naor_committer.cpp
    test/test_queue.cpp
    test/test_misc.cpp
)

add_executable(test.exe ${TEST_SOURCES})
target_include_directories(test.exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test.exe SYSTEM PRIVATE ${INCLUDES})
target_link_libraries(test.exe
    battleship
    ${LIBRARIES}
    GTest::GTest GTest::Main
)

install(TARGETS battleship.exe DESTINATION ".")
install(TARGETS test.exe DESTINATION ".")
