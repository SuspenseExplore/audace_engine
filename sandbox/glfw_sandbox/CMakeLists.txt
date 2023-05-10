cmake_minimum_required(VERSION 3.18.1)

project("sandbox_glfw")

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../engine_src)
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(
	sandbox_glfw
	include/glad.c
	main.cpp
	${ENGINE_DIR}/AuLogger.cpp
	${ENGINE_DIR}/renderer/DataBuffer.cpp
	${ENGINE_DIR}/renderer/ShaderProgram.cpp
	${ENGINE_DIR}/renderer/VertexAttribute.cpp
	${ENGINE_DIR}/renderer/VertexArray.cpp
)

target_include_directories(
	sandbox_glfw
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/glad
	${CMAKE_CURRENT_SOURCE_DIR}/include/KHR
	${ENGINE_DIR}
	${ENGINE_DIR}/glfw_platform
	${ENGINE_DIR}/include
)

add_library(
	spdlog
	STATIC
	IMPORTED
)
set_target_properties(
	spdlog
	PROPERTIES
	IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/spdlogd.lib
)

add_library(
	glfw3
	SHARED
	IMPORTED
)
set_target_properties(
	glfw3
	PROPERTIES
	IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib
)

target_link_libraries(
	sandbox_glfw
	PUBLIC
	glfw3
)