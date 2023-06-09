cmake_minimum_required(VERSION 3.25.0)

project(ImForge)

if((NOT MSVC) AND WIN32)
    message("[WARN] Compiling for windows without MSVC is not recommended, <guiddef.h> (required) will likely not be found.")
endif()

add_subdirectory("external")

file(GLOB_RECURSE SRC_FILES
    CMAKE_CONFIGURE_DEPENDS
    src/main.cpp
)

file(GLOB_RECURSE INCLUDE_FILES
    CMAKE_CONFIGURE_DEPENDS
    include/*/*.hpp
)

add_executable(ImForge
    ${SRC_FILES}
    ${INCLUDE_FILES}    
)

target_include_directories(ImForge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_dependencies(ImForge 
    SDL3::SDL3
    imgui
    spdlog::spdlog
)

target_link_libraries(ImForge PRIVATE 
    SDL3::SDL3
    imgui
    spdlog::spdlog
)