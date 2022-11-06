cmake_minimum_required(VERSION 3.23)
project(islands_circuit_designer)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp file_actions.cpp)
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(lib/json)
add_subdirectory(lib/raylib)
add_subdirectory(lib/raygui/projects/cmake)

add_library(tinyfiledialogs STATIC IMPORTED)
set_target_properties(tinyfiledialogs PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/tinyfiledialogs/tinyfiledialogs.c)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib raygui nlohmann_json::nlohmann_json tinyfiledialogs)