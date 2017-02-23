configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/ABY.CMakeLists.txt.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ABY.CMakeLists.txt"
    @ONLY
)

ExternalProject_Add(ABY
    DEPENDS MIRACL
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/ABY"
    INSTALL_DIR "${Battleship_INSTALL_PREFIX}"
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/ABY.CMakeLists.txt"
        "<SOURCE_DIR>/CMakeLists.txt"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/extern/OTExtension/ot"
        "<SOURCE_DIR>/src/abycore/ot"
    CMAKE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
		-DBUILD_EXE=ON
)
