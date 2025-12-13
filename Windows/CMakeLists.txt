cmake_minimum_required(VERSION 3.15)
project(RobinHood VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# For multi-config generators (VS), also set per-config directories
foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Find Vulkan SDK
# First try to use VULKAN_SDK environment variable
if(DEFINED ENV{VULKAN_SDK})
    set(VULKAN_SDK_PATH $ENV{VULKAN_SDK})
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
    
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib/vulkan-1.lib")
    else()
        set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib32/vulkan-1.lib")
    endif()
    
    message(STATUS "Using Vulkan SDK from environment: ${VULKAN_SDK_PATH}")
    set(Vulkan_FOUND TRUE)
else()
    find_package(Vulkan)
    if(NOT Vulkan_FOUND)
        message(WARNING "Vulkan SDK not found! Please install Vulkan SDK or set VULKAN_SDK environment variable.")
        message(WARNING "Download from: https://vulkan.lunarg.com/")
        message(WARNING "Attempting to use hardcoded paths as fallback...")
        
        # Try common Vulkan SDK installation paths
        set(VULKAN_POSSIBLE_PATHS
            "C:/VulkanSDK/1.3.290.0"
            "C:/VulkanSDK/1.3.280.0"
            "C:/VulkanSDK/1.3.275.0"
            "C:/VulkanSDK/1.4.328.1"
        )
        
        foreach(VULKAN_PATH ${VULKAN_POSSIBLE_PATHS})
            if(EXISTS "${VULKAN_PATH}/Include/vulkan/vulkan.h")
                set(VULKAN_SDK_PATH ${VULKAN_PATH})
                set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
                
                if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib/vulkan-1.lib")
                else()
                    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib32/vulkan-1.lib")
                endif()
                
                set(Vulkan_FOUND TRUE)
                message(STATUS "Found Vulkan SDK at: ${VULKAN_SDK_PATH}")
                break()
            endif()
        endforeach()
        
        if(NOT Vulkan_FOUND)
            message(FATAL_ERROR "Could not find Vulkan SDK! Please install it from https://vulkan.lunarg.com/")
        endif()
    endif()
endif()

# ImGui sources
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/dependencies/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_vulkan.cpp
    ${IMGUI_DIR}/imgui_impl_win32.cpp
)

set(IMGUI_HEADERS
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui_impl_vulkan.h
    ${IMGUI_DIR}/imgui_impl_win32.h
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h
)

# MinHook sources
set(MINHOOK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/dependencies/minhook)
set(MINHOOK_SOURCES
    ${MINHOOK_DIR}/buffer.c
    ${MINHOOK_DIR}/hook.c
    ${MINHOOK_DIR}/trampoline.c
    ${MINHOOK_DIR}/hde/hde32.c
    ${MINHOOK_DIR}/hde/hde64.c
)

set(MINHOOK_HEADERS
    ${MINHOOK_DIR}/MinHook.h
    ${MINHOOK_DIR}/buffer.h
    ${MINHOOK_DIR}/trampoline.h
    ${MINHOOK_DIR}/hde/hde32.h
    ${MINHOOK_DIR}/hde/hde64.h
    ${MINHOOK_DIR}/hde/pstdint.h
    ${MINHOOK_DIR}/hde/table32.h
    ${MINHOOK_DIR}/hde/table64.h
)

# Project sources
set(PROJECT_SOURCES
    src/dllmain.cpp
    src/hooks/hooks.cpp
    src/hooks/backend/vulkan/hook_vulkan.cpp
    src/menu/menu.cpp
    src/console/console.cpp
)

set(PROJECT_HEADERS
    src/backend.hpp
    src/hooks/hooks.hpp
    src/hooks/backend/vulkan/hook_vulkan.hpp
    src/menu/menu.hpp
    src/console/console.hpp
)

# Shared sources from Shared directory
file(GLOB_RECURSE SHARED_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/../Shared/src/*.cpp"
)

# Create the DLL
add_library(${PROJECT_NAME} SHARED
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${SHARED_SOURCES}
    ${IMGUI_SOURCES}
    ${IMGUI_HEADERS}
    ${MINHOOK_SOURCES}
    ${MINHOOK_HEADERS}
)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "powrprof"
    SUFFIX ".dll"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# For Debug configuration, keep the original name with configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "RobinHood_Debug"
    )
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dependencies
    ${CMAKE_CURRENT_SOURCE_DIR}/../Shared/src
    ${IMGUI_DIR}
    ${MINHOOK_DIR}
    ${Vulkan_INCLUDE_DIRS}
)

# Preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ImTextureID=ImU64
    ENABLE_BACKEND_VULKAN
    _CONSOLE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# Compiler options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W3
        /MP  # Multi-processor compilation
        /permissive-
    )
    
    # Release optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${PROJECT_NAME} PRIVATE
            /O2
            /Ob2
            /Oi
            /Ot
            /Oy
            /GL  # Whole program optimization
        )
        target_link_options(${PROJECT_NAME} PRIVATE
            /LTCG  # Link time code generation
            /OPT:REF
            /OPT:ICF
        )
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -O3
            -ffunction-sections
            -fdata-sections
        )
    endif()
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Vulkan_LIBRARIES}
    dwmapi
)

# Windows-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WINDOWS
        _USRDLL
    )
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Print configuration info
message(STATUS "=================================")
message(STATUS "RobinHood Configuration")
message(STATUS "=================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Vulkan Include: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Vulkan Libraries: ${Vulkan_LIBRARIES}")
message(STATUS "=================================")
