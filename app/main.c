#include <stdio.h>
#include <stdlib.h>

#include <kyu/kyu.h>
#include <kyu/shader.h>
#include <kyu/mesh.h>

#define WIDTH 1024
#define HEIGHT 640

static GLuint vao;
static GLuint vbo;

static float vertices[] = {
  -0.5f, -0.5f, 0.f,
   0.5f, -0.5f, 0.f,
   0.f,   0.5f, 0.f,
};

static float colors[] = {
  0.f, 0.f, 1.f,
  0.f, 1.f, 0.f,
  1.f, 0.f, 0.f,
};

static GLuint program;

static void
init()
{
  size_t size, offset;

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
  program = read_shaders("shaders/base_vertex.glsl", "shaders/base_fragment.glsl");
}

static void
quit()
{
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

static void
render()
{
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int
main()
{
  kyu_app app = kyu_init(WIDTH, HEIGHT, "Kyu v" KYU_VERSION, init, quit, render);

  return kyu_run(&app);
}
