cmake_minimum_required(VERSION 3.25.0)

# --- SDL ---
set(SDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

add_subdirectory(${SDL_DIR})

# --- ImGui ---
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SRC
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
)

target_link_libraries(imgui PRIVATE SDL3::SDL3)

# --- spdlog --- 
set(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
add_subdirectory(${SPDLOG_DIR})