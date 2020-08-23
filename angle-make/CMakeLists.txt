cmake_minimum_required(VERSION 3.16.0)

project(angle VERSION 0.1.0)

file(STRINGS ${PROJECT_SOURCE_DIR}/angle-source-files.txt ANGLE_SOURCE_FILES)

include_directories(${PROJECT_SOURCE_DIR}/../angle/src CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/include CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/src/common CACHE PATH)
include_directories(${PROJECT_SOURCE_DIR}/../angle/src/common/third_party/base CACHE PATH)

add_library(angle STATIC ${ANGLE_SOURCE_FILES})
