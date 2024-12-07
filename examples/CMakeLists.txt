cmake_minimum_required(VERSION 3.10)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/common/appicon.rc")

# Example 001 - Hello World
add_executable(Ex001_HelloWorld WIN32 Ex001_HelloWorld.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(Ex001_HelloWorld PRIVATE gdfx)

