cmake_minimum_required(VERSION 3.10.2)

project(mavlink-access)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(SOURCES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/mavlink_connection.hpp"
    "${SRC_DIR}/mavlink_connection.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(MAVSDK REQUIRED)

add_library(imgui STATIC
    ${LIB_DIR}/imgui/imgui.cpp
    ${LIB_DIR}/imgui/imgui_draw.cpp
    ${LIB_DIR}/imgui/imgui_demo.cpp
    ${LIB_DIR}/imgui/imgui_tables.cpp
    ${LIB_DIR}/imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${LIB_DIR}/imgui)

add_library(imgui_backend STATIC
    ${LIB_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${LIB_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_backend PUBLIC
    ${LIB_DIR}/imgui
    ${LIB_DIR}/imgui/backends
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(imgui_backend imgui glfw OpenGL::GL)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE MAVSDK::mavsdk imgui_backend)

if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W2)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Configuring Debug build")
    add_compile_options(-g)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()