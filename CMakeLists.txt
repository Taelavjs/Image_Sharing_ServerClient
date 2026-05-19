cmake_minimum_required(VERSION 4.2)
project(DbChatServer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(server
        server/server.c
        server/connection.c
        connection_util.h
        fileUtils/file_util.h
        connection_util.c
)

target_include_directories(server PRIVATE
        ${CMAKE_SOURCE_DIR}/ImageUtil
)

add_executable(client
        client/client.c
        client/connection.h
        client/connection.c
        connection_util.h
        fileUtils/file_util.h
        connection_util.c
)

target_include_directories(client PRIVATE
        ${CMAKE_SOURCE_DIR}/ImageUtil
)