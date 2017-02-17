ExternalProject_Add(nlohmann_json
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/json"
    INSTALL_DIR "${Battleship_INSTALL_PREFIX}"
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DBuildTests=OFF
)
