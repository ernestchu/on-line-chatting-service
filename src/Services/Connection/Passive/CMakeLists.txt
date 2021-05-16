set(HDIR "${CMAKE_SOURCE_DIR}/include/Services/Connection/Passive")

add_library(PassiveSock STATIC PassiveSock.cpp ${HDIR}/PassiveSock.hpp)
add_library(PassiveTCP STATIC PassiveTCP.cpp ${HDIR}/PassiveTCP.hpp)
add_library(PassiveUDP STATIC PassiveUDP.cpp ${HDIR}/PassiveUDP.hpp)

target_include_directories(PassiveSock PUBLIC ${HDIR})
target_include_directories(PassiveTCP PUBLIC ${HDIR})
target_include_directories(PassiveUDP PUBLIC ${HDIR})

target_link_libraries(PassiveSock PUBLIC Errexit)
target_link_libraries(PassiveTCP PUBLIC PassiveSock)
target_link_libraries(PassiveUDP PUBLIC PassiveSock)
