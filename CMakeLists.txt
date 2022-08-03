cmake_minimum_required(VERSION 3.22)
project(LaPrisionMuseo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(src include/json/include)
set(SRC_FILES
    src/Program.cpp

    src/components/Animator.cpp
    src/components/AspectRatio.cpp
    src/components/Internationalization.cpp

    src/login/LoginScene.cpp

    src/network/DebugNetwork.cpp
    src/network/NullNetwork.cpp
    src/network/SocketIONetwork.cpp

    src/room/RoomCamera.cpp
    src/room/RoomSceneNode.cpp

    src/scene/Scene.cpp
    src/scene/SceneNode.cpp
    src/scene/nodes/BackgroundNode.cpp
    src/scene/nodes/ClickableText.cpp

    src/Resources.cpp

    src/widgets/Cursor.cpp

    src/world/WorldScene.cpp
        src/splash/SplashScene.cpp src/splash/SplashScene.hpp)

set(SRC_IMGUI
    libs/imgui/imgui.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui-sfml/imgui-SFML.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${SRC_IMGUI})

# LIBRARY - SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics audio system window REQUIRED)

# LIBRARY - SOCKET.IO
find_package(OpenSSL REQUIRED)
add_subdirectory(libs/socket.io-client-cpp)
include_directories(libs/socket.io-client-cpp/src)

# LIBRARY - IMGUI AND IMGUI-SFML
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
add_subdirectory(libs/imgui-sfml)
include_directories(libs/imgui libs/imgui-sfml)

# LIBRARY TGUI
find_package(TGUI 0.10 REQUIRED)

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} 
    sfml-graphics
    sfml-audio
    sfml-system
    sioclient_tls
    ImGui-SFML::ImGui-SFML
    TGUI::TGUI
    ${CMAKE_THREAD_LIBS_INIT}
)

target_compile_options(${PROJECT_NAME} BEFORE PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/binaries $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
