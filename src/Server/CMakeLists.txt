set(HDIR "${CMAKE_SOURCE_DIR}/include/Server")

add_library(AbstractServer STATIC AbstractServer.cpp ${HDIR}/AbstractServer.hpp)
add_library(SingleThreadServer STATIC SingleThreadServer.cpp ${HDIR}/SingleThreadServer.hpp)
add_library(MultiThreadServer STATIC MultiThreadServer.cpp ${HDIR}/MultiThreadServer.hpp)

target_include_directories(AbstractServer PUBLIC ${HDIR})

target_compile_features(AbstractServer PUBLIC cxx_std_17)

target_link_libraries(AbstractServer PUBLIC PassiveTCP MessageWrapper)
target_link_libraries(SingleThreadServer PUBLIC AbstractServer)
target_link_libraries(MultiThreadServer PUBLIC AbstractServer)

