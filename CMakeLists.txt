cmake_minimum_required(VERSION 3.22)
project(LaPrisionMuseo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(src include/json/include)
set(SRC_FILES
    src/Program.cpp

    src/components/Animator.cpp
    src/components/AspectRatio.cpp

    src/network/DebugNetwork.cpp
    src/network/NullNetwork.cpp
    src/network/SocketIONetwork.cpp

    src/player/Player.cpp

    src/scene/Scene.cpp
    src/scene/SceneNode.cpp
    src/scene/nodes/BackgroundNode.cpp
    src/scene/nodes/ClickableText.cpp
    src/scene/nodes/Text.cpp

    src/scenes/login/LoginScene.cpp
    src/scenes/splash/SplashScene.cpp 
    src/scenes/splash/SplashNode.cpp 
    src/scenes/world/WorldScene.cpp
    src/scenes/world/room/RoomCamera.cpp
    src/scenes/world/room/RoomSceneNode.cpp

    src/system/Internationalization.cpp
        src/system/ResourcesManager.cpp

    src/widgets/Cursor.cpp
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

# LIBRARY - ZLIB
find_package(ZLIB 1.2.12 REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

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
    ${ZLIB_LIBRARIES}
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

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
