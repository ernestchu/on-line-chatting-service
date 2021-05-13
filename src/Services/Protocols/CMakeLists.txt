set(HDIR "${CMAKE_SOURCE_DIR}/include/Services/Protocols")

add_library(MessageWrapper INTERFACE ${HDIR}/MessageWrapper.hpp)

target_include_directories(MessageWrapper INTERFACE ${HDIR})

