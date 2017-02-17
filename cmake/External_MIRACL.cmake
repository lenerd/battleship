execute_process(
    COMMAND uname -m
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE ARCHITECTURE
)

if("${ARCHITECTURE}" STREQUAL "x86_64")
    set(MIRACL_MAKE "linux64")
else()
    set(MIRACL_MAKE "linux")
endif()

ExternalProject_Add(MIRACL
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/MIRACL"
    INSTALL_DIR "${Battleship_INSTALL_PREFIX}"
    CONFIGURE_COMMAND find "<SOURCE_DIR>/" -type d -name ".git" -prune -o -type f -exec cp {} "<BINARY_DIR>/" "$<SEMICOLON>"
    COMMAND sed -i -e "s/^rm /rm -f /" "<BINARY_DIR>/${MIRACL_MAKE}"

    BUILD_COMMAND bash "./${MIRACL_MAKE}"
    COMMAND find "<BINARY_DIR>/" -type f -name "*.o" -exec ar rc miracl_aux.a {} "$<SEMICOLON>"

    INSTALL_COMMAND mkdir -p "<INSTALL_DIR>/include/miracl_lib"
    COMMAND find "<BINARY_DIR>/" -type f -name "*.h" -exec cp {} "<INSTALL_DIR>/include/miracl_lib/" "$<SEMICOLON>"
    COMMAND mkdir -p "<INSTALL_DIR>/lib"
    COMMAND cp "<BINARY_DIR>/miracl.a" "<INSTALL_DIR>/lib/libmiracl.a"
    COMMAND cp "<BINARY_DIR>/miracl_aux.a" "<INSTALL_DIR>/lib/libmiracl_aux.a"
    COMMAND mkdir -p "<INSTALL_DIR>/include/dummy_include_dir"
)
