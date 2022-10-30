cmake_minimum_required(VERSION 3.23)
project(islands_circuit_designer)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp file_actions.cpp external/tinyfiledialogs.c)
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 4.2.0
    )
    FetchContent_MakeAvailable(raylib)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
    FetchContent_MakeAvailable(nlohmann_json)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS islands_circuit_designer RUNTIME DESTINATION bin)
include(CPack)
