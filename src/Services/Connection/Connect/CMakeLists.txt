set(HDIR "${CMAKE_SOURCE_DIR}/include/Services/Connection/Connect")

add_library(ConnectSock STATIC ConnectSock.cpp ${HDIR}/ConnectSock.hpp)
add_library(ConnectTCP STATIC ConnectTCP.cpp ${HDIR}/ConnectTCP.hpp)
add_library(ConnectUDP STATIC ConnectUDP.cpp ${HDIR}/ConnectUDP.hpp)

target_include_directories(ConnectSock PUBLIC ${HDIR})
target_include_directories(ConnectTCP PUBLIC ${HDIR})
target_include_directories(ConnectUDP PUBLIC ${HDIR})

target_compile_features(ConnectSock PUBLIC cxx_std_17)
target_compile_features(ConnectTCP PUBLIC cxx_std_17)
target_compile_features(ConnectUDP PUBLIC cxx_std_17)

target_link_libraries(ConnectSock PUBLIC Errexit)
target_link_libraries(ConnectTCP PUBLIC ConnectSock)
target_link_libraries(ConnectUDP PUBLIC ConnectSock)
