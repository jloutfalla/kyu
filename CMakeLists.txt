cmake_minimum_required(VERSION 3.16...3.18 FATAL_ERROR)

set(OpenGL_GL_PREFERENCE GLVND)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(GLAD_GENERATOR "c$<$<CONFIG:Debug>:-debug>" CACHE STRING "aze" FORCE)
add_subdirectory("libs/glad")
add_subdirectory("libs/glfw")

set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

project(kyu VERSION 0.1.0 LANGUAGES C)

if(MSVC)
  set(KYU_BUILD_SHARED OFF CACHE BOOL "Build Kyu as a shared library" FORCE)
else()
  option(KYU_BUILD_SHARED "Build Kyu as a shared library" ON)
endif()

find_package(OpenGL REQUIRED)

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
  "src/kyu/graphics/gl.c"
  "src/kyu/graphics/mesh.c"
  "src/kyu/graphics/shader.c"
  )

list(TRANSFORM LIB_FILES
  REPLACE "^src/" "include/"
  OUTPUT_VARIABLE LIB_HEADERS)

list(TRANSFORM LIB_HEADERS
  REPLACE "c$" "h")

list(APPEND LIB_HEADERS "include/glad/glad.h")

list(APPEND LIB_FILES
  # Utils
  "src/kyu/utils/glfw_utility.c"
  "src/kyu/utils/glfw_utility.h"
  )

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

add_custom_command(OUTPUT "${PROJECT_SOURCE_DIR}/include/glad/glad.h"
  DEPENDS glad
  COMMAND cp -r "${GLAD_INCLUDE_DIRS}/KHR" "${PROJECT_SOURCE_DIR}/include"
  COMMAND cp -r "${GLAD_INCLUDE_DIRS}/glad" "${PROJECT_SOURCE_DIR}/include"
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
  COMMENT "Getting GLAD header files"
  VERBATIM)

add_library(kyu "${KYU_LIB_TYPE}" "${LIB_FILES}" "${LIB_HEADERS}")

set_target_properties(kyu
  PROPERTIES
  C_EXTENSION OFF
  C_STANDARD 99)

if(MSVC)
  set_property(TARGET kyu PROPERTY C_STANDARD 11)
  target_compile_options(kyu PRIVATE /W4 /WX /wd4127 /wd5105)
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

target_link_libraries(kyu
  PRIVATE
  glad
  glfw

  PUBLIC
  OpenGL::GL)
#--------------------------------------------------------------------#

add_subdirectory(app)
