cmake_minimum_required(VERSION 3.21)

project(oberpc)

file(GLOB_RECURSE OBERPC_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE OBERPC_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(oberpc SHARED ${OBERPC_HEADERS} ${OBERPC_SOURCES})

target_include_directories(oberpc
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

set(BUILD_PLAYER OFF CACHE BOOL "")
set(BUILD_DEV OFF CACHE BOOL "")
set(BUILD_TOOLKIT OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

add_subdirectory(ObEngine)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT oberpc)

target_link_libraries(oberpc ObEngineCore)

add_subdirectory(extlibs/httplib)

target_link_libraries(oberpc httplib::httplib)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)
