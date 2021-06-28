cmake_minimum_required(VERSION 3.16...3.18 FATAL_ERROR)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_subdirectory(
  "libs/glfw"
  )

if(MSVC)
  add_compile_options(/Wall /WX)
else()
  add_compile_options(-W -Wall -Wextra -pedantic -Werror)
endif()

project(kyu VERSION 0.1.0 LANGUAGES C)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)

set(LIB_FILES
  "src/kyu/kyu.c"
  "src/kyu/utils.c"
  "src/kyu/base.c"
  "src/kyu/vector.c"
  "src/kyu/mesh.c"
  "src/kyu/shader.c"
  "src/kyu/glfw_utility.h"
  "src/kyu/glfw_utility.c"
  )

file(COPY "shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

configure_file("${PROJECT_SOURCE_DIR}/include/kyu/version.h.in"
  "${PROJECT_BINARY_DIR}/include/kyu/version.h"
  )

#------------------------ Kyu lib ------------------------------------#
add_library("${PROJECT_NAME}" "${LIB_FILES}")
target_compile_features("${PROJECT_NAME}" PRIVATE c_std_99)
set_target_properties("${PROJECT_NAME}" PROPERTIES C_EXTENSION OFF)

target_include_directories("${PROJECT_NAME}"
  PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_BINARY_DIR}/include"

  PRIVATE
  "${PROJECT_SOURCE_DIR}/src/kyu")

target_link_libraries("${PROJECT_NAME}"
  PRIVATE
  glfw

  PUBLIC
  OpenGL::GL
  GLEW::GLEW
  )
#--------------------------------------------------------------------#

add_subdirectory(app)
