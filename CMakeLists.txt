cmake_minimum_required(VERSION 3.22)

# Required so QPM-generated cmake can set the package version / MOD_ID / VERSION defines.
project(UIEnhancements VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# QPM (with "cmake": true in qpm.json) generates the toolchain + QuestHook package config.
include(${CMAKE_CURRENT_SOURCE_DIR}/extern/includes/qpm/cmake/qpm.cmake OPTIONAL RESULT_VARIABLE QPM_CMAKE)
if(NOT QPM_CMAKE)
  # Fallback for older layouts / first configure before restore finishes writing files.
  message(STATUS "qpm.cmake not found yet – run 'qpm restore' then reconfigure")
endif()

file(GLOB_RECURSE MOD_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${MOD_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_options(${PROJECT_NAME} PRIVATE -fPIC -fvisibility=hidden -fexceptions -frtti)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  MOD_ID="${PROJECT_NAME}"
  VERSION="${PROJECT_VERSION}"
  GIT_COMMIT="local"
)

# Prefer the modern QuestHook imported target when available.
if(TARGET QuestHook::QuestHook)
  target_link_libraries(${PROJECT_NAME} PRIVATE QuestHook::QuestHook)
elseif(TARGET beatsaber-hook_static)
  target_link_libraries(${PROJECT_NAME} PRIVATE beatsaber-hook_static)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME "UIEnhancements"
  PREFIX "lib"
  SUFFIX ".so"
)
