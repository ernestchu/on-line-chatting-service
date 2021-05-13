 add_executable(test test.cpp)
 target_link_libraries(test PUBLIC SingleThreadServer)
 target_compile_features(test PUBLIC cxx_std_17)

