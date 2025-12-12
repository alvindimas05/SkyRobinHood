# Shared CMakeLists.txt Template
# This can be used if you want to build shared code as a separate library

cmake_minimum_required(VERSION 3.10)
project(RobinHoodShared)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Shared source files
file(GLOB_RECURSE SHARED_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Shared header files
file(GLOB_RECURSE SHARED_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

# Create a static library (or INTERFACE library if header-only)
add_library(RobinHoodShared STATIC
    ${SHARED_SOURCES}
    ${SHARED_HEADERS}
)

# Include directories
target_include_directories(RobinHoodShared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# TODO: Add ImGui dependency
# find_package(imgui REQUIRED)
# target_link_libraries(RobinHoodShared PUBLIC imgui)

# Platform-specific compile definitions
if(ANDROID)
    target_compile_definitions(RobinHoodShared PUBLIC PLATFORM_ANDROID)
elseif(WIN32)
    target_compile_definitions(RobinHoodShared PUBLIC PLATFORM_WINDOWS)
endif()

# Optional: Export for use in parent projects
# set(ROBINHOOD_SHARED_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src PARENT_SCOPE)
