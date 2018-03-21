ExternalProject_Add(Crypto++
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/cryptopp"
    INSTALL_DIR "${Battleship_INSTALL_PREFIX}"
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/extern/cryptopp-cmake/CMakeLists.txt"
        "${PROJECT_SOURCE_DIR}/extern/cryptopp-cmake/cryptopp-config.cmake"
        "<SOURCE_DIR>/"
    CMAKE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DBUILD_TESTING=OFF
        -DBUILD_SHARED=OFF
        -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
)
