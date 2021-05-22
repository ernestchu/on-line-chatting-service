add_executable(Client Client.cpp)
add_executable(Server Server.cpp)

target_link_directories(Client PUBLIC /usr/local/lib)
target_link_directories(Server PUBLIC /usr/local/lib)
target_link_libraries(Client PUBLIC WindowController openabe)
target_link_libraries(Server PUBLIC SingleThreadServer MultiThreadServer openabe)

add_custom_command(
    TARGET Client 
    POST_BUILD 
    COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:Client>" ../exec/Client
    COMMENT "Copying Client to exec directory"
)
add_custom_command(
    TARGET Server
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:Server>" ../exec/Server
    COMMENT "Copying Server to exec directory"
)
 
