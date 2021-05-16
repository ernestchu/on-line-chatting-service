set(HDIR "${CMAKE_SOURCE_DIR}/include/Services/Connection")

add_library(Errexit STATIC Errexit.cpp ${HDIR}/Errexit.hpp)

target_include_directories(Errexit PUBLIC ${HDIR})

add_subdirectory(Connect)
add_subdirectory(Passive)
