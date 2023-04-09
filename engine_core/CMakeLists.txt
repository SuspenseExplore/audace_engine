cmake_minimum_required(VERSION 3.18.1)

project("engine_core")

add_library(
	engine_core
	STATIC
	RenderContext.cpp
)

target_include_directories(
	engine_core
	PUBLIC
	${PLATFORM_DIR}
)

add_subdirectory(
	${PLATFORM_DIR}
	${PLATFORM_DIR}/build
)

target_link_libraries(
	engine_core
	PUBLIC
	audace_platform
)