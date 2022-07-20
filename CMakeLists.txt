cmake_minimum_required(VERSION 3.22)
project(LaPrisionMuseo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(src)
set(SRC_FILES
    src/Program.cpp

    src/login/LoginSceneNode.cpp

    src/network/DebugNetwork.cpp
    src/network/NullNetwork.cpp
    src/network/SocketIONetwork.cpp

    src/room/RoomCamera.cpp
    src/room/RoomSceneNode.cpp

    src/scene/Scene.cpp
    src/scene/SceneNode.cpp

    src/widgets/QuadAspectRatio.cpp
)

set(SRC_IMGUI
    libs/imgui/imgui.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui-sfml/imgui-SFML.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${SRC_IMGUI})

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics audio system window REQUIRED)


find_package(OpenSSL REQUIRED)
add_subdirectory(libs/socket.io-client-cpp)
include_directories(libs/socket.io-client-cpp/src)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
add_subdirectory(libs/imgui-sfml)
include_directories(libs/imgui libs/imgui-sfml)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-system sioclient_tls ImGui-SFML::ImGui-SFML)
