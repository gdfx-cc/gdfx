cmake_minimum_required(VERSION 3.10)

project(gdfx)

if (GDFX_USE_SYSTEM_SDL)
	find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
else()
	# SDL as a submodule in src/SDL
	add_subdirectory(src/SDL EXCLUDE_FROM_ALL)
endif()

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

file(GLOB SOURCES
	"src/gdfx/game/*.cpp"
	"src/gdfx/game/*.hpp"
	"src/gdfx/graphics/*.cpp"
	"src/gdfx/graphics/*.hpp"
	"src/gdfx/math/*.cpp"
	"src/gdfx/math/*.hpp"
	"src/gdfx/platform/*.cpp"
	"src/gdfx/platform/*.hpp"
	"src/gdfx/*.cpp"
	"src/gdfx/*.hpp"
)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${SOURCES})

add_library(${PROJECT_NAME}
	STATIC
	${SOURCES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")

add_subdirectory(examples)
