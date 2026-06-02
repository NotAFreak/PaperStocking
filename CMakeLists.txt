# CMakeLists.txt for the engine
cmake_minimum_required(VERSION 3.16.0)
project(PaperStocking VERSION 0.1.0 LANGUAGES C CXX)

# Set options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(RMLUI_BACKEND SDL_GL3)

# SDL. What the FUCK are you DOING?
set(BUILD_SHARED_LIBS OFF)

# prevent linking by absolute path (WHY IS THAT A THING BY DEFAULT????)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}")
endif("${isSystemDir}" STREQUAL "-1")

# Create the library
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*
)
add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCES})
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
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/json)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/box2d)

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
    RMLUI_NUM_MSAA_SAMPLES=16
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
    rmlui_backend_SDL_GL3
    EnTT::EnTT
    nlohmann_json
    SQLite::SQLite3
    box2d::box2d
)