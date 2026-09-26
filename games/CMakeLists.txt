cmake_minimum_required(VERSION 3.10)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/common/appicon.rc")

# SandBox
add_executable(SandBox WIN32 sandbox/Game.cpp ${APP_ICON_RESOURCE_WINDOWS})
target_link_libraries(SandBox PRIVATE gdfx)
