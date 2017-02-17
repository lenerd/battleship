ExternalProject_Add(Beast
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/Beast"
    INSTALL_DIR "${Battleship_INSTALL_PREFIX}"
    CONFIGURE_COMMAND ""
    UPDATE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                         "<SOURCE_DIR>/include"
                         "<INSTALL_DIR>/include"
)
