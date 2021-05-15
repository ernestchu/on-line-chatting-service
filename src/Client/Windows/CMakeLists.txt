set(HDIR "${CMAKE_SOURCE_DIR}/include/Client/Windows")

# ncurses.h
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_library(WindowConfig INTERFACE ${HDIR}/WindowConfig.hpp)
add_library(LoginWindow STATIC LoginWindow.cpp ${HDIR}/LoginWindow.hpp)
add_library(InputWindow STATIC InputWindow.cpp ${HDIR}/InputWindow.hpp)
add_library(ReceivingWindow STATIC ReceivingWindow.cpp ${HDIR}/ReceivingWindow.hpp)
add_library(Windows INTERFACE ${HDIR}/Windows.hpp)

target_include_directories(WindowConfig INTERFACE ${HDIR})

target_compile_features(LoginWindow PUBLIC cxx_std_17)
target_compile_features(InputWindow PUBLIC cxx_std_17)
target_compile_features(ReceivingWindow PUBLIC cxx_std_17)

target_link_libraries(WindowConfig INTERFACE ${CURSES_LIBRARIES})
target_link_libraries(LoginWindow PUBLIC WindowConfig)
target_link_libraries(InputWindow PUBLIC WindowConfig)
target_link_libraries(ReceivingWindow PUBLIC WindowConfig)
target_link_libraries(Windows INTERFACE LoginWindow InputWindow ReceivingWindow)

