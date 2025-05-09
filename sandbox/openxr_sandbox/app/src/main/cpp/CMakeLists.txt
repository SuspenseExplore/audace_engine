# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("openxr_sandbox")
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../engine_src)
set(APP_DIR ${ENGINE_DIR}/../sandbox/app_src)

# add android's native app glue
add_library(native_app_glue STATIC
		${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
		openxr_sandbox

		# Sets the library as a shared library.
		SHARED

		# Provides a relative path to your source file(s).
		main.cpp
		${ENGINE_DIR}/stb_image.cpp
		${ENGINE_DIR}/AuLogger.cpp
		${ENGINE_DIR}/include/imgui/imgui.cpp
		${ENGINE_DIR}/include/imgui/imgui_draw.cpp
		${ENGINE_DIR}/include/imgui/imgui_tables.cpp
		${ENGINE_DIR}/include/imgui/imgui_widgets.cpp
		${ENGINE_DIR}/android_platform/imgui/imgui_impl_android.cpp
		${ENGINE_DIR}/android_platform/imgui/imgui_impl_opengl3.cpp
		${ENGINE_DIR}/android_platform/ImageData.cpp
		${ENGINE_DIR}/android_platform/FileLoader.cpp
		${ENGINE_DIR}/input/InputDevices.cpp
		${ENGINE_DIR}/content/AssetStore.cpp
		${ENGINE_DIR}/content/ByteBuffer.cpp
		${ENGINE_DIR}/content/ModelLoader.cpp
		${ENGINE_DIR}/content/JsonSerializer.cpp
		${ENGINE_DIR}/renderer/DataBuffer.cpp
		${ENGINE_DIR}/renderer/ShaderProgram.cpp
		${ENGINE_DIR}/renderer/VertexAttribute.cpp
		${ENGINE_DIR}/renderer/VertexArray.cpp
		${ENGINE_DIR}/renderer/Texture2d.cpp
		${ENGINE_DIR}/renderer/Mesh.cpp
		${ENGINE_DIR}/renderer/Sprite.cpp
		${ENGINE_DIR}/renderer/Shapes.cpp
		${ENGINE_DIR}/renderer/text/BitmapFont.cpp
		${ENGINE_DIR}/geom/VoxelTerrainGen.cpp
		${ENGINE_DIR}/scene/Scene.cpp
		openxr/OpenxrSwapchain.cpp
		openxr/OpenxrView.cpp
		openxr/OpenxrContext.cpp
		openxr/HmdCamera.cpp
		EglWindow.cpp
		AppController.cpp
		${APP_DIR}/scene/MainScene.cpp
		${APP_DIR}/scene/ProcTerrainScene.cpp
		)
include_directories(
		.
		lib
)

target_compile_definitions(
		openxr_sandbox
		PRIVATE
		IMGUI_IMPL_OPENGL_ES3
)

add_custom_command(
		TARGET openxr_sandbox POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${ENGINE_DIR}/../sandbox/assets
		${CMAKE_CURRENT_SOURCE_DIR}/../assets
)

# add the oculus loader for openxr
add_library(openxr_loader SHARED IMPORTED)
set_property(
		TARGET
		openxr_loader
		PROPERTY
		IMPORTED_LOCATION
		${CMAKE_CURRENT_SOURCE_DIR}/lib/libopenxr_loader.so
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
		log-lib

		# Specifies the name of the NDK library that
		# you want CMake to locate.
		log)

target_include_directories(
		openxr_sandbox
		PRIVATE
		${ANDROID_NDK}/sources/android/native_app_glue
		${ENGINE_DIR}
		${ENGINE_DIR}/include
		${ENGINE_DIR}/include/imgui
		${ENGINE_DIR}/android_platform
		${ENGINE_DIR}/android_platform/imgui
		${APP_DIR}
)

add_library(
		freetype
		STATIC
		IMPORTED
)
set_target_properties(
		freetype
		PROPERTIES
		IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}../../../../libs/${ANDROID_ABI}/libfreetype2-static.a
)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

include (FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries( # Specifies the target library.
		openxr_sandbox
		android
		native_app_glue
		EGL
		GLESv3
		openxr_loader
		freetype
		nlohmann_json::nlohmann_json

		# Links the target library to the log library
		# included in the NDK.
		${ANDROID_LIBRARY}
		${log-lib})