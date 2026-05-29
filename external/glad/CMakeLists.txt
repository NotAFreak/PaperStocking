# make glad work with cmake
cmake_minimum_required(VERSION 3.16.0)
project(GLAD VERSION 2.0.0 LANGUAGES C)

option(GLAD_BUILD_SHARED "Build GLAD as a shared library" ON)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
)

if(${GLAD_BUILD_SHARED})
    add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES})
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)