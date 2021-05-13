set(HDIR "${CMAKE_SOURCE_DIR}/include/Services/Connection")

add_library(Errexit STATIC Errexit.cpp ${HDIR}/Errexit.hpp)

target_include_directories(Errexit PUBLIC ${HDIR})

target_compile_features(Errexit PUBLIC cxx_std_17)

add_subdirectory(Connect)
add_subdirectory(Passive)
