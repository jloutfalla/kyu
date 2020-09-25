/* kyu -- simple app for 3D rendering with OpenGL
   Copyright (C) 2020 Jean-Baptiste Loutfalla

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "glfw_utility.h"

#define ERR_EXIT(STR)                           \
  {                                             \
    fprintf(stderr, "[ERROR] - %s\n", (STR));   \
    exit(EXIT_FAILURE);                         \
  }                                             \


#define WIDTH 1024
#define HEIGHT 640

GLuint vao;
GLuint vbo;

float vertices[] = {
  -0.5f, -0.5f, 0.f,
   0.5f, -0.5f, 0.f,
   0.f,   0.5f, 0.f,
};

float colors[] = {
  0.f, 0.f, 1.f,
  0.f, 1.f, 0.f,
  1.f, 0.f, 0.f,
};

const char* vertex = ""
  "#version 330\n"
  "layout(location = 0) in vec3 position;\n"
  "layout(location = 1) in vec3 color;\n"
  "\n"
  "out vec3 o_color;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  o_color = color;\n"
  "  gl_Position = vec4(position, 1.f);\n"
  "}\n";

const char* fragment = ""
  "#version 330\n"
  "in vec3 o_color;\n"
  "out vec4 frag_color;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  frag_color = vec4(o_color, 1.f);\n"
  "}\n";

GLuint program;

void init()
{
  size_t size, offset;
  GLuint vertex_shader, fragment_shader;
  int success;
  char info[512];

  /* VAO and VBO */
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size = sizeof(vertices) + sizeof(colors);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

  offset = 0;
  size = sizeof(vertices);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) offset);
  glEnableVertexAttribArray(0);

  offset += size;
  size = sizeof(colors);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, colors);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) offset);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  /* Shaders */
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(vertex_shader, 512, NULL, info);
      fprintf(stderr,
              "[GL ERROR] - Vertex shader compilation failed\n%s\n",
              info);
    }

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(fragment_shader, 512, NULL, info);
      fprintf(stderr,
              "[GL ERROR] - Fragment shader compilation failed\n%s\n",
              info);
    }

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
    {
      glGetProgramInfoLog(program, 512, NULL, info);
      fprintf(stderr, "[GL ERROR] - Program linking failed\n%s\n", info);
    }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader); 
}

void quit()
{
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

int main(int argc, char *argv[])
{
  GLFWwindow* window = NULL;
  GLenum err;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE

  
  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      ERR_EXIT("Can't create a window...");
    }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resize_callback);
  glfwSetKeyCallback(window, input_callback);
  
  err = glewInit();
  if (err != GLEW_OK)
    {
      glfwTerminate();
      ERR_EXIT("Can't initialize GLEW...");
    }  

  init();
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(program);
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  quit();
  glfwTerminate();
  return EXIT_SUCCESS;
}
