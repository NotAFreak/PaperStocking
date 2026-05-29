# CMakeLists.txt for the engine
cmake_minimum_required(VERSION 3.16.0)
project(PaperStocking VERSION 0.1.0 LANGUAGES C CXX)

# Set options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the library
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*
)
add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCES}
        include/PaperStocking/debug/debugTool.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/tclap)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL)
# link
target_link_libraries(${PROJECT_NAME} PUBLIC
    TCLAP
    GLAD
    SDL3::SDL3
)