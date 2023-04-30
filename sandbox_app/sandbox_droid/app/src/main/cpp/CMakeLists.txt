# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("sandbox_droid")
add_library(native_app_glue STATIC
		${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../..)
set(ENGINE_ROOT_DIR ${APP_DIR}/..)
set(ENGINE_DIR ${ENGINE_ROOT_DIR}/engine_core)
set(PLATFORM_DIR ${ENGINE_ROOT_DIR}/android_platform/app/src/main/cpp)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        sandbox_droid

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${PLATFORM_DIR}/AuLogger.cpp
		${PLATFORM_DIR}/EglWindow.cpp
		${PLATFORM_DIR}/EntryPoint.cpp
		${ENGINE_DIR}/RenderContext.cpp
		${ENGINE_DIR}/renderer/DataBuffer.cpp
		${ENGINE_DIR}/renderer/VertexArray.cpp
		${ENGINE_DIR}/renderer/ShaderProgram.cpp
		${APP_DIR}/SandboxApp.cpp)

add_subdirectory(
        D:/spdlog
        D:/spdlog/build
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
        sandbox_droid
        PUBLIC
		${ANDROID_NDK}/sources/android/native_app_glue
		D:/spdlog/include
		${PLATFORM_DIR}
        ${ENGINE_DIR}
		${APP_DIR}
)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        sandbox_droid
        android
        native_app_glue
		EGL
		GLESv3

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})