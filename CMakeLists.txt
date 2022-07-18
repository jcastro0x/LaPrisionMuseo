cmake_minimum_required(VERSION 3.22)
project(LaPrisionMuseo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(SRC_FILES
    src/main.cpp
)

set(SFML_STATIC_LIBRARIES TRUE)
add_executable(${PROJECT_NAME} ${SRC_FILES})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_subdirectory(libs/socket.io-client-cpp)
include_directories(libs/socket.io-client-cpp/src)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sioclient)
