cmake_minimum_required(VERSION 3.16...3.18 FATAL_ERROR)

project(kyu VERSION 0.1.0 LANGUAGES C)

set(BUILD_PS2 OFF)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Generic")
  if("${CMAKE_CROSSCOMPILING}")
    set(BUILD_PS2 ON)
  endif()
else()
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
  set(GLAD_GENERATOR "c$<$<CONFIG:Debug>:-debug>" CACHE STRING "aze" FORCE)

  set(OpenGL_GL_PREFERENCE GLVND)
  
  add_subdirectory("libs/glad")
  set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
  
  add_subdirectory("libs/glfw")
endif()

if(MSVC)
  set(KYU_BUILD_SHARED OFF CACHE BOOL "Build Kyu as a shared library" FORCE)
elseif(NOT "${BUILD_PS2}")
  option(KYU_BUILD_SHARED "Build Kyu as a shared library" ON)
endif()

set(LIB_FILES
  "src/kyu/kyu.c"

  # Core
  "src/kyu/core/utils.c"
  "src/kyu/core/base.c"
  "src/kyu/core/file.c"

  # Math
  "src/kyu/math/vector.c"
  "src/kyu/math/matrix.c"

  # Graphics
  "src/kyu/graphics/mesh.c"
  )

if(NOT BUILD_PS2)
  list(APPEND LIB_FILES
    "src/kyu/graphics/gl.c"
    "src/kyu/graphics/shader.c")
endif()

list(TRANSFORM LIB_FILES
  REPLACE "^src/" "include/"
  OUTPUT_VARIABLE LIB_HEADERS)

list(TRANSFORM LIB_HEADERS
  REPLACE "c$" "h")

if(NOT BUILD_PS2)
  list(APPEND LIB_HEADERS "include/glad/glad.h")
  list(APPEND LIB_FILES
    # Utils
    "src/kyu/utils/glfw_utility.c"
    "src/kyu/utils/glfw_utility.h"
    )

endif()

file(COPY "shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

configure_file("${PROJECT_SOURCE_DIR}/include/kyu/core/version.h.in"
  "${PROJECT_BINARY_DIR}/include/kyu/core/version.h"
  )

#------------------------ Kyu lib ------------------------------------#
set(KYU_LIB_TYPE "SHARED")
if(NOT KYU_BUILD_SHARED)
  set(KYU_LIB_TYPE "STATIC")  
endif()

if(NOT "${BUILD_PS2}")
  add_custom_command(OUTPUT "${PROJECT_SOURCE_DIR}/include/glad/glad.h"
    DEPENDS glad
    COMMAND cp -r "${GLAD_INCLUDE_DIRS}/KHR" "${PROJECT_SOURCE_DIR}/include"
    COMMAND cp -r "${GLAD_INCLUDE_DIRS}/glad" "${PROJECT_SOURCE_DIR}/include"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Getting GLAD header files"
    VERBATIM)
endif()

add_library(kyu "${KYU_LIB_TYPE}" "${LIB_FILES}" "${LIB_HEADERS}")

target_compile_features(kyu PRIVATE c_std_99)

if(MSVC)
  target_compile_options(kyu PRIVATE /W4 /WX /wd4127 /wd5105 /MP)
else()
  target_compile_options(kyu PRIVATE -W -Wall -Wextra -pedantic -Werror)
endif()

if(MSVC)
  target_compile_definitions(kyu
    PRIVATE
    "_CRT_SECURE_NO_WARNINGS" # don't raise a warning while using fopen
    
    PUBLIC
    "__KYU_WIN__")
elseif(UNIX)
  target_compile_definitions(kyu
    PUBLIC
    "__KYU_UNIX__")
elseif(BUILD_PS2)
  target_compile_definitions(kyu
    PUBLIC
    "__KYU_PS2__")
endif()
  
find_library(MATH_LIB m)
if(MATH_LIB)
  target_link_libraries(kyu PUBLIC "${MATH_LIBRARY}")
endif()

target_include_directories(kyu
  PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_BINARY_DIR}/include"

  PRIVATE
  "${PROJECT_SOURCE_DIR}/src/kyu")

if(NOT "${BUILD_PS2}")
  find_package(OpenGL REQUIRED)
  
  target_link_libraries(kyu
    PRIVATE
    glad
    glfw

    PUBLIC
    OpenGL::GL)

  add_subdirectory(app)
else()
  find_library(KERNEL kernel REQUIRED)
  find_library(DMA dma REQUIRED)
  find_library(GRAPH graph REQUIRED)
  find_library(DRAW draw REQUIRED)
  
  target_link_libraries(kyu
    PUBLIC
    "${KERNEL}"
    "${DMA}"
    "${GRAPH}"
    "${DRAW}")
  
endif()
