cmake_minimum_required(VERSION 3.20)
project(rpcsx)

option(WITH_RPCSX "Enable RPCSX" ON)
option(WITH_RPCS3 "Enable RPCS3" OFF)
option(WITH_RPCS3_QT_UI "Enable RPCS3 UI" OFF)
option(WITHOUT_OPENGL "Disable OpenGL" OFF)
option(WITHOUT_OPENGLEW "Disable OpenGLEW" OFF)
option(WITHOUT_OPENAL "Disable OpenAL" OFF)
option(COMPILE_FFMPEG "Compile FFmpeg" ON)

# rpcs3 options
option(USE_NATIVE_INSTRUCTIONS "USE_NATIVE_INSTRUCTIONS makes rpcs3 compile with -march=native, which is useful for local builds, but not good for packages." ON)
option(WITH_LLVM "Enable usage of LLVM library" ON)
option(BUILD_LLVM "Build LLVM from git submodule" OFF)
option(STATIC_LINK_LLVM "Link against LLVM statically. This will get set to ON if you build LLVM from the submodule." OFF)
option(USE_FAUDIO "FAudio audio backend" ON)
option(USE_LIBEVDEV "libevdev-based joystick support" ON)
option(USE_DISCORD_RPC "Discord rich presence integration" OFF)
option(USE_SYSTEM_ZLIB "Prefer system ZLIB instead of the builtin one" ON)
option(USE_VULKAN "Vulkan render backend" ON)
option(USE_PRECOMPILED_HEADERS "Use precompiled headers" OFF)
option(USE_SDL "Enables SDL input handler" OFF)
option(USE_SYSTEM_SDL "Prefer system SDL instead of the builtin one" ON)
option(USE_SYSTEM_FFMPEG "Prefer system ffmpeg instead of the prebuild one" OFF)
option(USE_SYSTEM_OPENAL "Prefer system OpenAL instead of the prebuild one" ON)
option(USE_SYSTEM_CURL "Prefer system Curl instead of the prebuild one" ON)
option(USE_SYSTEM_OPENCV "Prefer system OpenCV instead of the builtin one" ON)
option(HAS_MEMORY_BREAKPOINTS "Add support for memory breakpoints to the interpreter" OFF)
option(USE_LTO "Use LTO for building" ON)


if (NOT WITH_RPCS3)
    set(WITHOUT_OPENGL on)
    set(WITHOUT_OPENGLEW on)
    set(WITHOUT_OPENAL on)
    set(WITH_LLVM off)
    set(USE_FAUDIO off)
    set(USE_LIBEVDEV off)
    set(USE_DISCORD_RPC off)
    set(USE_SYSTEM_ZLIB off)
    set(USE_VULKAN off)
    set(USE_PRECOMPILED_HEADERS off)
    set(USE_SDL off)
    set(USE_SYSTEM_SDL off)
    set(USE_SYSTEM_FFMPEG off)
    set(USE_SYSTEM_OPENAL off)
    set(USE_SYSTEM_CURL off)
    set(USE_SYSTEM_OPENCV off)
    set(HAS_MEMORY_BREAKPOINTS off)
endif()

set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_RPATH_USE_ORIGIN on)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11)
        message(FATAL_ERROR "RPCS3 requires at least gcc-11.")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12.0)
        message(FATAL_ERROR "RPCS3 requires at least clang-12.0.")
    endif()
endif()

include(CheckCXXCompilerFlag)

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    set(USE_DISCORD_RPC OFF CACHE BOOL "Discord RPC is only available with single-config generator" FORCE)
else()
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "No build type selected, default to Release")
        set(CMAKE_BUILD_TYPE "Release")
    endif()
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug" AND NOT MSVC)
    add_compile_definitions(_DEBUG)
endif()

if(MSVC)
    option(USE_MSVC_STATIC_CRT "Use static MSVC C runtime" OFF)

    if(NOT IS_MULTI_CONFIG)
        if(NOT(CMAKE_BUILD_TYPE MATCHES "Release" AND USE_MSVC_STATIC_CRT))
            set(USE_DISCORD_RPC OFF CACHE BOOL "Discord RPC is only available in Release and static CRT build." FORCE)
        endif()
    endif()

    if(USE_MSVC_STATIC_CRT)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()

    add_compile_options(/MP)
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()

if(APPLE AND CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif()

if(MSVC)
    add_compile_options(/wd4530 /utf-8) # C++ exception handler used, but unwind semantics are not enabled
endif()


add_subdirectory(3rdparty EXCLUDE_FROM_ALL)
add_subdirectory(rx EXCLUDE_FROM_ALL)

include(3rdparty/llvm/CMakeLists.txt)

if (NOT RX_TAG)
    set(RX_TAG 0)
endif()

if (NOT RX_TAG_VERSION)
    set(RX_TAG_VERSION 0)
endif()

target_compile_definitions(rx PRIVATE
    RX_TAG=${RX_TAG}
    RX_TAG_VERSION=${RX_TAG_VERSION}
)

if (WITH_RPCSX)
    find_package(nlohmann_json CONFIG)

    function(add_precompiled_vulkan_spirv target)
        add_library(${target} INTERFACE)
        set(SPIRV_GEN_ROOT_DIR "spirv-gen/include/")
        set(SPIRV_GEN_DIR "${SPIRV_GEN_ROOT_DIR}/shaders")

        cmake_path(ABSOLUTE_PATH SPIRV_GEN_ROOT_DIR BASE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} NORMALIZE OUTPUT_VARIABLE outputrootdir)
        cmake_path(ABSOLUTE_PATH SPIRV_GEN_DIR BASE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} NORMALIZE OUTPUT_VARIABLE outputdir)
        file(MAKE_DIRECTORY ${outputrootdir})
        file(MAKE_DIRECTORY ${outputdir})
        target_include_directories(${target} INTERFACE ${outputrootdir})

        foreach(input IN LISTS ARGN)
            cmake_path(GET input FILENAME inputname)
            cmake_path(REPLACE_EXTENSION inputname LAST_ONLY .h OUTPUT_VARIABLE outputname)
            cmake_path(APPEND outputdir ${outputname} OUTPUT_VARIABLE outputpath)
            cmake_path(REMOVE_EXTENSION inputname LAST_ONLY OUTPUT_VARIABLE varname)

            string(REPLACE "." "_" varname ${varname})
            string(PREPEND varname "spirv_")

            add_custom_command(
                OUTPUT ${outputpath}
                COMMAND $<TARGET_FILE:glslang::glslang-standalone> -V --target-env vulkan1.2 --vn "${varname}" -o "${outputpath}" "${CMAKE_CURRENT_SOURCE_DIR}/${input}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${input}" glslang::glslang-standalone
                COMMENT "Generating ${outputname}..."
            )

            set(subtarget ".${target}-subtarget-${outputname}")
            add_custom_target(${subtarget} DEPENDS ${outputpath})
            add_dependencies(${target} ${subtarget})
        endforeach()
    endfunction()

    function(target_base_address target address)
        set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE off)

        if(CMAKE_CXX_COMPILER_LINKER_ID MATCHES "^(LLD|MOLD)$")
            target_link_options(${target} PUBLIC "LINKER:--image-base=${address}")
        else()
            target_link_options(${target} PUBLIC "LINKER:-Ttext-segment,${address}")
        endif()
    endfunction()

    add_subdirectory(tools)

    add_subdirectory(orbis-kernel)
endif()

add_subdirectory(rpcsx)

if (WITH_RPCS3)
    include(ConfigureCompiler)
    include(CheckFunctionExists)

    add_subdirectory(rpcs3)
    add_subdirectory(ps3fw)
endif()

if (NOT ANDROID AND WITH_RPCS3_QT_UI AND WITH_RPCS3)
    add_subdirectory(rpcs3qt-legacy)
endif()
