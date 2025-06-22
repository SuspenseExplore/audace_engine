cmake_minimum_required(VERSION 3.18.1)

project("audace_engine_test")

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(
	audace_engine_test
	${ENGINE_DIR}/stb_image.cpp
	${ENGINE_DIR}/AuLogger.cpp
	${ENGINE_DIR}/include/b64/cencode.c
	${ENGINE_DIR}/include/b64/cdecode.c
	${ENGINE_DIR}/glfw_platform/include/glad.c
	${ENGINE_DIR}/glfw_platform/FileAccessGlfw.cpp
	${ENGINE_DIR}/content/ByteBuffer.cpp
	${ENGINE_DIR}/content/JsonSerializer.cpp
	${ENGINE_DIR}/content/ModelLoader.cpp
	${ENGINE_DIR}/content/gltf/GltfLoader.cpp
	${ENGINE_DIR}/content/AssetStore.cpp
	${ENGINE_DIR}/content/gltf/_test/GltfLoader_test.cpp
	${ENGINE_DIR}/renderer/Mesh.cpp
	${ENGINE_DIR}/renderer/Shapes.cpp
	${ENGINE_DIR}/renderer/ShaderProgram.cpp
	${ENGINE_DIR}/renderer/DataBuffer.cpp
	${ENGINE_DIR}/renderer/VertexAttribute.cpp
	${ENGINE_DIR}/renderer/VertexArray.cpp
	${ENGINE_DIR}/renderer/Texture2d.cpp
	${ENGINE_DIR}/renderer/material/Material.cpp
	${ENGINE_DIR}/renderer/Sprite.cpp
	${ENGINE_DIR}/scene/graph/RotationAnimation.cpp
)

target_include_directories(
	audace_engine_test
	PRIVATE
	${TEST_DIR}
	${ENGINE_DIR}
	${ENGINE_DIR}/include
	${ENGINE_DIR}/include/b64
	${ENGINE_DIR}/content
	${ENGINE_DIR}/content/gltf
	${ENGINE_DIR}/content/gltf/_test
	${ENGINE_DIR}/renderer
	${ENGINE_DIR}/renderer/material
	${ENGINE_DIR}/glfw_platform
	${ENGINE_DIR}/glfw_platform/include
	${ENGINE_DIR}/glfw_platform/include/glad
	${ENGINE_DIR}/glfw_platform/include/KHR
)

include (FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(
	audace_engine_test
	PUBLIC
	nlohmann_json::nlohmann_json
)