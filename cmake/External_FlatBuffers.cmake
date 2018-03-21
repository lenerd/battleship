ExternalProject_Add(flatbuffers
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/flatbuffers"
    INSTALL_DIR "${Battleship_INSTALL_PREFIX}"
    CMAKE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        # flatc is currently only installed in Release mode
        # https://github.com/google/flatbuffers/issues/4511
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DFLATBUFFERS_BUILD_TESTS=OFF
        -DFLATBUFFERS_BUILD_FLATLIB=OFF
        -DFLATBUFFERS_BUILD_FLATHASH=OFF
        -DCMAKE_CXX_COMPILER:PATH=/usr/bin/g++
        -DCMAKE_C_COMPILER:PATH=/usr/bin/gcc
)
