cmake_minimum_required(VERSION 3.18.1)

project("sandbox_windows")

set(ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(PLATFORM_DIR ${ENGINE_ROOT_DIR}/glfw_platform)
set(ENGINE_DIR ${ENGINE_ROOT_DIR}/engine_core)

add_executable(
	sandbox_windows
	${PLATFORM_DIR}/include/glad.c
	${PLATFORM_DIR}/AuLogger.cpp
	${PLATFORM_DIR}/EntryPoint.cpp
	${PLATFORM_DIR}/GameWindow.cpp
	${ENGINE_DIR}/RenderContext.cpp
	${ENGINE_DIR}/renderer/DataBuffer.cpp
	${ENGINE_DIR}/renderer/VertexArray.cpp
	${ENGINE_DIR}/renderer/ShaderProgram.cpp
	${APP_DIR}/SandboxApp.cpp
)

target_include_directories(
	sandbox_windows
	PRIVATE
	${PLATFORM_DIR}
	${PLATFORM_DIR}/include
	${PLATFORM_DIR}/include/glad
	${PLATFORM_DIR}/include/KHR
	${ENGINE_DIR}
	${ENGINE_DIR}/include
	D:/spdlog/include
)

add_subdirectory(
	D:/spdlog
	D:/spdlog/build
)

add_library(
	glfw3
	SHARED
	IMPORTED
)
set_target_properties(
	glfw3
	PROPERTIES
	IMPORTED_IMPLIB ${PLATFORM_DIR}/lib/glfw3.lib
)

target_link_libraries(
	sandbox_windows
	PUBLIC
	glfw3
)