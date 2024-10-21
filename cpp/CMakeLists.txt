cmake_minimum_required(VERSION 3.22.1)
project(NativeMap)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
    find_package(JNI REQUIRED)
    include_directories(${JAVA_INCLUDE_PATH})
endif()

find_library(log-lib log)

include_directories(libraries/glm)
include_directories(libraries/glm/gtc)
include_directories(libraries/glm/gtx)
include_directories(libraries/glm/detail)
include_directories(libraries/glx)
include_directories(views)
include_directories(utils)
include_directories(utils/android)
include_directories(renderers)
include_directories(renderers/gl)
include_directories(shaders)
include_directories(shaders/gl)

set(SRCS
    "views/Canvas.cpp"
    "views/CanvasMap.cpp"

    "utils/Utils.cpp"
    "utils/android/OSUtils.cpp"
    "utils/android/AndroidMapDelegate.cpp"
    "utils/android/AndroidAssetStorage.cpp"

    "renderers/MapTile.cpp"
    "renderers/MapTileRenderer.cpp"

    "renderers/gl/GlMapTileRenderer.cpp"
    "renderers/gl/GlUtils.cpp"
    "renderers/gl/GlMapTile.cpp"
)


add_library(nativemap SHARED ${SRCS})

target_link_libraries(nativemap android ${log-lib} EGL GLESv3)

