cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(websocket)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./base SRC_BASE)
aux_source_directory(./client CLIENT)
aux_source_directory(./server SERVER)
aux_source_directory(./server/base MONGOOSE)

include_directories(
    base
    client
    server
)

link_libraries(pthread)

add_executable(client ${CLIENT})
add_executable(server ${SERVER} ${BASE} ${MONGOOSE})