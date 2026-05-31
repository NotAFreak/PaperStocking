# CMakeLists.txt for the engine
cmake_minimum_required(VERSION 3.16.0)
project(PaperStocking VERSION 0.1.0 LANGUAGES C CXX)

# Set options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(RMLUI_BACKEND SDL_GL3)

# SDL. What the FUCK are you DOING?
set(BUILD_SHARED_LIBS OFF)

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
find_package(Freetype REQUIRED)
find_package(SQLite3 REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/tclap)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/RmlUi)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/entt)

# Create RmlUi backend
set(RMLUI_BACKEND_PATH ${RmlUi_SOURCE_DIR}/Backends)
add_library(rmlui_backend_SDL_GL3 INTERFACE)
target_sources(rmlui_backend_SDL_GL3 INTERFACE
        ${RMLUI_BACKEND_PATH}/RmlUi_Platform_SDL.cpp
        ${RMLUI_BACKEND_PATH}/RmlUi_Renderer_GL3.cpp
)
target_include_directories(rmlui_backend_SDL_GL3 INTERFACE ${RMLUI_BACKEND_PATH})
target_compile_definitions(rmlui_backend_SDL_GL3 INTERFACE
        RMLUI_SDL_VERSION_MAJOR=3
        RMLUI_NUM_MSAA_SAMPLES=4
        RMLUI_GL3_CUSTOM_LOADER=<glad/gl.h>
)
target_link_libraries(${PROJECT_NAME} PUBLIC
        rmlui_backend_SDL_GL3
)

# link
target_link_libraries(${PROJECT_NAME} PUBLIC
    TCLAP
    GLAD
    SDL3::SDL3
    SDL3_image::SDL3_image
    RmlUi::RmlUi
    RmlUi::Debugger
    Freetype::Freetype
    EnTT::EnTT
    SQLite::SQLite3
    rmlui_backend_SDL_GL3
)