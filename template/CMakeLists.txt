cmake_minimum_required(VERSION 3.21)

project(GDFXApp)

set(GDFX_DIR "${CMAKE_CURRENT_LIST_DIR}/../gdfx" CACHE PATH "Path to the GDFX framework")

add_subdirectory(
	"${GDFX_DIR}"
	"${CMAKE_BINARY_DIR}/gdfx-build"
)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/app/appicon.rc")
set(APP_ICON_RESOURCE_MACOS "${CMAKE_CURRENT_SOURCE_DIR}/app/appicon.icns")

file(GLOB SOURCES CONFIGURE_DEPENDS
	"src/*.cpp"
	"src/*.hpp"
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

add_executable(${PROJECT_NAME}
	WIN32
	MACOSX_BUNDLE
	${SOURCES}
	${APP_ICON_RESOURCE_WINDOWS}
	${APP_ICON_RESOURCE_MACOS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE gdfx)
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

set_source_files_properties(${APP_ICON_RESOURCE_MACOS} PROPERTIES
	MACOSX_PACKAGE_LOCATION "Resources"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
	MACOSX_BUNDLE_GUI_IDENTIFIER "cc.gdfx.${PROJECT_NAME}"
	MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
	MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
	MACOSX_BUNDLE_ICON_FILE "appicon.icns"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/data"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
)

if (APPLE)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/data"
			"$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Resources/data"
	)
endif()

if (UNIX AND NOT APPLE)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_SOURCE_DIR}/app/${PROJECT_NAME}.desktop"
			"$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.desktop"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_SOURCE_DIR}/app/appicon.png"
			"$<TARGET_FILE_DIR:${PROJECT_NAME}>/appicon.png"
	)
endif()
