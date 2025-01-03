cmake_minimum_required(VERSION 3.10)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/common/appicon.rc")

# Example 001 - Hello World
add_executable(Ex001_HelloWorld WIN32 Ex001_HelloWorld.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex001_HelloWorld PRIVATE gdfx)

# Example 002 - Hello Image
add_executable(Ex002_HelloImage WIN32 Ex002_HelloImage.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex002_HelloImage PRIVATE gdfx)

# Example 003 - Hello Sound
add_executable(Ex003_HelloSound WIN32 Ex003_HelloSound.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex003_HelloSound PRIVATE gdfx)

# Example 004 - Hello Music
add_executable(Ex004_HelloMusic WIN32 Ex004_HelloMusic.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex004_HelloMusic PRIVATE gdfx)

# Example 005 - Hello Primitives
add_executable(Ex005_HelloPrimitives WIN32 Ex005_HelloPrimitives.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex005_HelloPrimitives PRIVATE gdfx)

# Example 006 - Hello Input
add_executable(Ex006_HelloInput WIN32 Ex006_HelloInput.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex006_HelloInput PRIVATE gdfx)

# Example 007 - Hello Animation
add_executable(Ex007_HelloAnimation WIN32 Ex007_HelloAnimation.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex007_HelloAnimation PRIVATE gdfx)
