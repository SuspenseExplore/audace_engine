cmake_minimum_required(VERSION 3.18.1)

project("sandbox_glfw")

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../engine_src)
set(PLATFORM_DIR ${ENGINE_DIR}/glfw_platform)
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../app_src)

add_executable(
	sandbox_glfw
	${PLATFORM_DIR}/include/glad.c
	${ENGINE_DIR}/stb_image.cpp
	main.cpp
	AppController.cpp
	${APP_DIR}/scene/MainScene.cpp
	${APP_DIR}/scene/NavigationScene.cpp
	${APP_DIR}/scene/SceneBuilder.cpp
	${APP_DIR}/scene/DragDropScene.cpp
	${APP_DIR}/scene/TextScene.cpp
	${APP_DIR}/scene/ProcTerrainScene.cpp
	${APP_DIR}/scene/GltfViewerScene.cpp
	${ENGINE_DIR}/AuLogger.cpp
	${ENGINE_DIR}/glfw_platform/GameWindow.cpp
	${ENGINE_DIR}/glfw_platform/KeyboardManager.cpp
	${ENGINE_DIR}/glfw_platform/MouseManager.cpp
	${ENGINE_DIR}/glfw_platform/FileAccessGlfw.cpp
	${ENGINE_DIR}/include/FastNoiseLite.h
	${ENGINE_DIR}/include/imgui/imgui.cpp
	${ENGINE_DIR}/include/imgui/imgui_demo.cpp
	${ENGINE_DIR}/include/imgui/imgui_draw.cpp
	${ENGINE_DIR}/include/imgui/imgui_tables.cpp
	${ENGINE_DIR}/include/imgui/imgui_widgets.cpp
	${ENGINE_DIR}/include/b64/cencode.c
	${ENGINE_DIR}/include/b64/cdecode.c
	${ENGINE_DIR}/util/TimeUtil.h
	${ENGINE_DIR}/glfw_platform/imgui/imgui_impl_glfw.cpp
	${ENGINE_DIR}/glfw_platform/imgui/imgui_impl_opengl3.cpp
	${ENGINE_DIR}/renderer/DataBuffer.cpp
	${ENGINE_DIR}/renderer/ShaderProgram.cpp
	${ENGINE_DIR}/renderer/VertexAttribute.cpp
	${ENGINE_DIR}/renderer/VertexArray.cpp
	${ENGINE_DIR}/renderer/Texture2d.cpp
	${ENGINE_DIR}/renderer/Mesh.cpp
	${ENGINE_DIR}/renderer/Sprite.cpp
	${ENGINE_DIR}/renderer/Shapes.cpp
	${ENGINE_DIR}/renderer/material/Material.cpp
	${ENGINE_DIR}/renderer/light/PointLight.h
	${ENGINE_DIR}/renderer/text/BitmapFont.cpp
	${ENGINE_DIR}/renderer/text/TextLabel.cpp
	${ENGINE_DIR}/content/ImageData.cpp
	${ENGINE_DIR}/content/gltf/GltfLoader.cpp
	${ENGINE_DIR}/content/ModelLoader.cpp
	${ENGINE_DIR}/content/AssetStore.cpp
	${ENGINE_DIR}/content/JsonSerializer.cpp
	${ENGINE_DIR}/editor/SpriteData.cpp
	${ENGINE_DIR}/editor/SceneData.cpp
	${ENGINE_DIR}/editor/SceneEditor.cpp
	${ENGINE_DIR}/editor/SpriteEditWindow.cpp
	${ENGINE_DIR}/geom/VoxelTerrainGen.cpp
	${ENGINE_DIR}/scene/Scene.cpp
	${ENGINE_DIR}/scene/SceneDescriptor.cpp
	${ENGINE_DIR}/scene/BaseCamera.h
	${ENGINE_DIR}/scene/ForwardCamera.cpp
	${ENGINE_DIR}/scene/BasicCameraController.cpp
	${ENGINE_DIR}/scene/graph/SceneGraph.cpp
	${ENGINE_DIR}/scene/graph/SceneGraphNode.cpp
	${ENGINE_DIR}/scene/graph/RotationAnimation.cpp
)

add_compile_definitions(AU_PLATFORM_GLFW)

target_include_directories(
	sandbox_glfw
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${PLATFORM_DIR}
	${PLATFORM_DIR}/include
	${PLATFORM_DIR}/include/glad
	${PLATFORM_DIR}/include/KHR
	${ENGINE_DIR}/glfw_platform/include/imgui
	${ENGINE_DIR}
	${ENGINE_DIR}/glfw_platform
	${ENGINE_DIR}/include
	${ENGINE_DIR}/include/imgui
	${ENGINE_DIR}/include/freetype
	${APP_DIR}
)

add_library(
	spdlog
	STATIC
	IMPORTED
)
set_target_properties(
	spdlog
	PROPERTIES
	IMPORTED_IMPLIB ${PLATFORM_DIR}/lib/spdlogd.lib
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

add_library(
	freetype
	SHARED
	IMPORTED
)
set_target_properties(
	freetype
	PROPERTIES
	IMPORTED_IMPLIB ${PLATFORM_DIR}/lib/freetype.lib
)

include (FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(
	sandbox_glfw
	PUBLIC
	glfw3
	freetype
	nlohmann_json::nlohmann_json
)