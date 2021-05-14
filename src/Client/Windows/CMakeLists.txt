set(HDIR "${CMAKE_SOURCE_DIR}/include/Client/Windows")

# ncurses.h
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_library(LoginWindow STATIC LoginWindow.cpp ${HDIR}/LoginWindow.hpp)
# add_library(ChatRoomWindow STATIC ChatRoomWindow.cpp ${HDIR}/ChatRoomWindow.hpp)
add_library(Windows INTERFACE ${HDIR}/Windows.hpp)

target_include_directories(LoginWindow PUBLIC ${HDIR})
# target_include_directories(ChatRoomWindow PUBLIC ${HDIR})
target_include_directories(Windows INTERFACE ${HDIR})

target_compile_features(LoginWindow PUBLIC cxx_std_17)
# target_compile_features(ChatRoomWindow PUBLIC cxx_std_17)

target_link_libraries(LoginWindow PUBLIC ${CURSES_LIBRARIES})
# target_link_libraries(ChatRoomWindow PUBLIC ${CURSES_LIBRARIES})
target_link_libraries(Windows INTERFACE LoginWindow)

