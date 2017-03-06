cmake_minimum_required(VERSION 3.0)
project(Battleship CXX)

set(CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/../cmake"
    "${PROJECT_SOURCE_DIR}/../extern/flatbuffers/CMake"
)

set(CMAKE_INCLUDE_PATH ${DEPENDENCY_PREFIX}/include)
set(CMAKE_LIBRARY_PATH ${DEPENDENCY_PREFIX}/lib ${DEPENDENCY_PREFIX}/lib64)

# Dependencies
find_package(Boost REQUIRED COMPONENTS program_options system)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(GTest)
find_library(CRYPTO++_LIBRARY libcryptopp.a)
if ((NOT CRYPTO++_LIBRARY) OR (NOT EXISTS ${CRYPTO++_LIBRARY}))
    message(FATAL_ERROR "Crypto++ not found")
else()
    message(STATUS "Found Crypto++: ${CRYPTO++_LIBRARY}")
endif()
find_library(GMP_LIBRARY gmp)
if ((NOT GMP_LIBRARY) OR (NOT EXISTS ${GMP_LIBRARY}))
    message(FATAL_ERROR "GMP not found")
else()
    message(STATUS "Found GMP: ${GMP_LIBRARY}")
endif()
find_library(GMPXX_LIBRARY gmpxx)
if ((NOT GMPXX_LIBRARY) OR (NOT EXISTS ${GMPXX_LIBRARY}))
    message(FATAL_ERROR "GMPXX not found")
else()
    message(STATUS "Found GMPXX: ${GMPXX_LIBRARY}")
endif()
find_path(MIRACL_INCLUDE_DIR miracl_lib/miracl.h)
if ((NOT MIRACL_INCLUDE_DIR) OR (NOT EXISTS ${MIRACL_INCLUDE_DIR}))
    message(FATAL_ERROR "MIRACL include dir not found")
else()
    message(STATUS "Found MIRACL include dir: ${MIRACL_INCLUDE_DIR}")
endif()
find_library(ABY_LIBRARY libaby.a)
if ((NOT ABY_LIBRARY) OR (NOT EXISTS ${ABY_LIBRARY}))
    message(FATAL_ERROR "ABY not found")
else()
    message(STATUS "Found ABY: ${ABY_LIBRARY}")
endif()
find_library(MIRACL_LIBRARY libmiracl.a)
if ((NOT MIRACL_LIBRARY) OR (NOT EXISTS ${MIRACL_LIBRARY}))
    message(FATAL_ERROR "MIRACL not found")
else()
    message(STATUS "Found MIRACL: ${MIRACL_LIBRARY}")
endif()
find_library(MIRACL_CXX_LIBRARY libmiracl_cxx.a)
if ((NOT MIRACL_CXX_LIBRARY) OR (NOT EXISTS ${MIRACL_CXX_LIBRARY}))
    message(FATAL_ERROR "MIRACL_CXX not found")
else()
    message(STATUS "Found MIRACL_CXX: ${MIRACL_CXX_LIBRARY}")
endif()

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_C_FLAGS} -Wall -Wextra -Wconversion -Wshadow -Weffc++")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")


set(BATTLESHIP_SOURCES
    circuits/aby_party_connection.cpp
    circuits/battleship_decider.cpp
    circuits/example_decider.cpp
    circuits/keccak.cpp
    circuits/trivial_decider.cpp
    crypto/committer.cpp
    crypto/hash_committer.cpp
    crypto/keccak.cpp
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
    ${MIRACL_INCLUDE_DIR}/dummy_include_dir
    ${CMAKE_CURRENT_BINARY_DIR}/generated_includes
    ${Boost_INCLUDE_DIR} ${CMAKE_INCLUDE_PATH}
)

set(LIBRARIES
    Boost::program_options Boost::system
    Threads::Threads
    ${CURSES_LIBRARIES}
    ${CRYPTO++_LIBRARY}
    ${ABY_LIBRARY}
    ${MIRACL_CXX_LIBRARY} ${MIRACL_LIBRARY}
    ${GMP_LIBRARY} ${GMPXX_LIBRARY}
    Threads::Threads
    OpenSSL::SSL OpenSSL::Crypto
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
    crypto/keccak_test_vectors.cpp
    test/test.cpp
    test/test_board.cpp
    test/test_dummy_connection.cpp
    test/test_game_communicator.cpp
    test/test_committer.cpp
    test/test_hash_committer.cpp
    test/test_keccak.cpp
    test/test_keccak_circuit.cpp
    test/test_naor_committer.cpp
    test/test_queue.cpp
    test/test_misc.cpp
    test/test_battleship_decider.cpp
    test/test_example_decider.cpp
    test/test_trivial_decider.cpp
)

add_executable(test.exe ${TEST_SOURCES})
target_include_directories(test.exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test.exe SYSTEM PRIVATE ${INCLUDES})
target_link_libraries(test.exe
    battleship
    ${LIBRARIES}
    GTest::GTest GTest::Main
)

add_executable(test_bd.exe circuits/test_bd.cpp)
target_include_directories(test_bd.exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test_bd.exe SYSTEM PRIVATE ${INCLUDES})
target_link_libraries(test_bd.exe battleship ${LIBRARIES})

install(TARGETS battleship.exe DESTINATION ".")
install(TARGETS test.exe DESTINATION ".")
