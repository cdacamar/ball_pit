cmake_minimum_required(VERSION 3.15)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

# WORKAROUND
# This is added so that CMake can recognize the .ixx extension as a module interface.
set(CMAKE_CXX_SYSROOT_FLAG_CODE "list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS ixx)")

project(
    "ball_pit"
    VERSION 0.1.0
    LANGUAGES CXX
)

# It is always easier to navigate in an IDE when projects are organized in folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(
    3rd_party/olcPixelGameEngine
    include
)

message("CMAKE_CXX_SOURCE_FILE_EXTENSIONS ${CMAKE_CXX_SOURCE_FILE_EXTENSIONS}")

add_executable(ball_pit
    3rd_party/olcPixelGameEngine/olcPixelGameEngine.h
    src/3rd_party/olcPixelGameEngine.cpp
    src/ball-pit.cpp
    src/physics/physics-ball.cpp
    src/physics/physics-engine.cpp
    modules/bridges/pge-bridge.ixx
    modules/physics/physics-ball.ixx
    modules/physics/physics-engine.ixx
    modules/physics/physics-utils.ixx
    modules/physics/physics.ixx
    modules/physics/quad-tree.ixx
    modules/util/basic-types.ixx
    modules/util/enum-utils.ixx
    modules/util/random-generator.ixx
    modules/util/stopwatch.ixx
    modules/util/util.ixx
    modules/world/world.ixx
)

# Only support statically linking for now.
target_compile_definitions(ball_pit PRIVATE
                            SEQUENTIAL_PROCESSING=0)

# Require c++20, this is better than setting CMAKE_CXX_STANDARD since it won't pollute other targets
# note : cxx_std_* features were added in CMake 3.8.2
target_compile_features(ball_pit PRIVATE cxx_std_20)

target_compile_options(ball_pit PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 /WX /permissive- /Z7 /MT /MP /Bt+>)

set_target_properties(ball_pit PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)