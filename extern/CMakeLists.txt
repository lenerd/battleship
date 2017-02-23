message(STATUS "Building external projects")

include(ExternalProject)

include(External_Beast)
include(External_Crypto++)
include(External_FlatBuffers)
include(External_json)
include(External_MIRACL)
include(External_OTExtension)
include(External_ABY)

add_dependencies(OTExtension MIRACL)
add_dependencies(ABY MIRACL)
