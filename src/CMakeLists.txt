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
    board_component.cpp
    board.cpp
    board_impl.cpp
    board_window.cpp
    committer.cpp
    dummy_connection.cpp
    game_communicator.cpp
    game.cpp
    game_screen.cpp
    hash_committer.cpp
    message_window.cpp
    naor_committer.cpp
    ncurses_interface.cpp
    ncurses_window.cpp
    options.cpp
    tcp_connection.cpp
    user_interface.cpp
    util.cpp
    web_interface.cpp
    websocket_connection.cpp
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
target_include_directories(battleship
    PRIVATE ${Boost_INCLUDE_DIR} ${CMAKE_INCLUDE_PATH}
)

add_executable(battleship.exe battleship.cpp)
target_link_libraries(battleship.exe
    battleship
    ${LIBRARIES}
)

set(TEST_SOURCES
    test.cpp
    test_board.cpp
    test_dummy_connection.cpp
    test_game_communicator.cpp
    test_committer.cpp
    test_hash_committer.cpp
    test_naor_committer.cpp
    test_queue.cpp
    test_misc.cpp
)

add_executable(test.exe ${TEST_SOURCES})
target_link_libraries(test.exe
    battleship
    ${LIBRARIES}
    GTest::GTest GTest::Main
)

install(TARGETS battleship.exe DESTINATION ".")
install(TARGETS test.exe DESTINATION ".")
