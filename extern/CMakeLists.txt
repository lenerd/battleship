message(STATUS "Building external projects")

include(ExternalProject)

include(External_Beast)
include(External_Crypto++)
include(External_FlatBuffers)
include(External_json)
include(External_MIRACL)
include(External_ENCRYPTO_utils)
include(External_OTExtension)
include(External_ABY)

add_dependencies(ENCRYPTO_utils MIRACL)
add_dependencies(OTExtension MIRACL)
add_dependencies(ABY MIRACL)
