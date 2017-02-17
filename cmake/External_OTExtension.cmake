configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/OTExtension.CMakeLists.txt.in"
    "${CMAKE_CURRENT_BINARY_DIR}/OTExtension.CMakeLists.txt"
    @ONLY
)

ExternalProject_Add(OTExtension
    DEPENDS MIRACL
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/OTExtension"
    INSTALL_DIR "${Battleship_INSTALL_PREFIX}"
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/OTExtension.CMakeLists.txt"
        "<SOURCE_DIR>/CMakeLists.txt"
    CMAKE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)