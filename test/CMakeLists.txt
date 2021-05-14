add_executable(cli cli.cpp)
add_executable(srv srv.cpp)

target_link_libraries(cli PUBLIC WindowController)
target_link_libraries(srv PUBLIC SingleThreadServer)

target_compile_features(cli PUBLIC cxx_std_17)
target_compile_features(cli PUBLIC cxx_std_17)

