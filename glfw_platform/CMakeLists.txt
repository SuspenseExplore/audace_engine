cmake_minimum_required(VERSION 3.18.1)

project("audace_platform")

add_library(
	audace_platform
	STATIC
	GameWindow.cpp
	include/glad.c
	EntryPoint.cpp
	AuLogger.cpp
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

add_subdirectory(
	D:/spdlog
	D:/spdlog/build
)

target_include_directories(
	audace_platform
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/glad
	${CMAKE_CURRENT_SOURCE_DIR}/include/KHR
	${ENGINE_DIR}
	D:/spdlog/include
)

target_link_libraries(
	audace_platform
	PUBLIC
	glfw3
	spdlog
)