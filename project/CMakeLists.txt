cmake_minimum_required(VERSION 3.16.0)

project(spglsl VERSION 0.1.0)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MACOSX_RPATH 1)

IF(EMSCRIPTEN)
  ADD_DEFINITIONS(-DEMSCRIPTEN -DANGLE_WITH_TSAN -DANGLE_ENABLE_NULL)
  add_definitions(-DSPIRV_EMSCRIPTEN)

  set(CMAKE_SYSTEM_NAME "Emscripten" CACHE STRING "CMAKE_SYSTEM_NAME" FORCE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --no-entry \
    --std=c++17 \
    -fno-rtti \
    -flto \
    -Wno-unused-command-line-argument \
    -s STRICT=1 \
    -s SAFE_HEAP=0 \
    -s EVAL_CTORS=1 \
    -s FILESYSTEM=0 \
    -s EXIT_RUNTIME=0 \
    -s NO_EXIT_RUNTIME=1 \
    -s SAFE_HEAP_LOG=0 \
    -s ABORTING_MALLOC=0 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 \
    $ENV{EMSCRIPTEN_CUSTOM_CXX_FLAGS} \
    --bind \
    -s MODULARIZE=1 -fno-exceptions -s DISABLE_EXCEPTION_CATCHING=1")
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/packages/spglsl/wasm CACHE PATH "WASM Build directory" FORCE)

ELSE()
  include_directories(${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/system CACHE PATH)
  include_directories(${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/system/include CACHE PATH)
  include_directories(${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/system/include/libcxx CACHE PATH)
  include_directories(${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/system/include/libc CACHE PATH)
  include_directories(${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/system/lib/libc/musl/arch/emscripten CACHE PATH)
  include_directories(${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/system/include/compat CACHE PATH)
ENDIF()

include_directories(${PROJECT_SOURCE_DIR}/.. CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/src CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/include CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/src/common CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/src/common/base CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/src/common/third_party CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/src/common/third_party/xxhash CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/third_party/zlib CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/third_party/zlib/google CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/cpp CACHE PATH)

# ######### angle library ##########
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../angle-make ${CMAKE_CURRENT_SOURCE_DIR}/../angle/build)

# ######### spglsl wasm ##########
file(GLOB_RECURSE SPGLSL_SRC_FILES CONFIGURE_DEPENDS cpp/spglsl/*.cpp cpp/spglsl-common/*.cpp)
add_executable(spglsl ${SPGLSL_SRC_FILES})

IF(EMSCRIPTEN)
  set_target_properties(spglsl PROPERTIES COMPILE_FLAGS "-fexceptions -s DISABLE_EXCEPTION_CATCHING=0 -s EXPORT_NAME=\"'spglsl'\"")
  set_target_properties(spglsl PROPERTIES LINK_FLAGS "-fexceptions -s DISABLE_EXCEPTION_CATCHING=0 -s EXPORT_NAME=\"'spglsl'\"")
ENDIF()

target_link_libraries(spglsl angle)
