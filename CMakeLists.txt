cmake_minimum_required(VERSION 3.10)

project(kyu)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -pedantic ${CMAKE_C_FLAGS}")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(include)

set(SRC_FILES
  src/kyu.c
  src/glfw_utility.c)

add_executable(${PROJECT_NAME}
  ${SRC_FILES}
  )

target_link_libraries(${PROJECT_NAME}
  OpenGL::GL
  glfw
  GLEW)
