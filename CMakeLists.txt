project(server)

cmake_minimum_required(VERSION 3.16)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin" )
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

include_directories("${PROJECT_SOURCE_DIR}/cJSON/include")
include_directories("${PROJECT_SOURCE_DIR}/utils_cpp/include")

add_subdirectory(cJSON)
add_subdirectory(utils_cpp)

add_executable(client src/client.c)
target_link_libraries(client "${PROJECT_SOURCE_DIR}/lib/libcjson.a"  "${PROJECT_SOURCE_DIR}/lib/libutils.a" m)
add_executable(server src/server.c)
target_link_libraries(server "${PROJECT_SOURCE_DIR}/lib/libcjson.a" "${PROJECT_SOURCE_DIR}/lib/libutils.a" m pthread)
