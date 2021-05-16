set(HDIR "${CMAKE_SOURCE_DIR}/include/Client")
find_package (Threads)

add_library(WindowController STATIC WindowController.cpp ${HDIR}/WindowController.hpp)
add_library(Network STATIC Network.cpp ${HDIR}/Network.hpp)

target_include_directories(WindowController PUBLIC ${HDIR})
target_include_directories(Network PUBLIC ${HDIR})

target_compile_features(WindowController PUBLIC cxx_std_17)
target_compile_features(Network PUBLIC cxx_std_17)

target_link_libraries(WindowController PUBLIC Windows Network Threads::Threads)
target_link_libraries(Network PUBLIC ConnectTCP MessageWrapper)

add_subdirectory(Windows)

