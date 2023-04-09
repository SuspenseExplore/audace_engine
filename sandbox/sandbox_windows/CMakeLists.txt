cmake_minimum_required(VERSION 3.18.1)

project("sandbox_windows")

set(GAME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../glfw_platform)
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../engine_core)

add_executable(
	sandbox_windows
	${GAME_DIR}/SandboxApp.cpp
)

add_subdirectory(
	${ENGINE_DIR}
	${ENGINE_DIR}/build
)

target_link_libraries(
	sandbox_windows
	PUBLIC
	engine_core
)