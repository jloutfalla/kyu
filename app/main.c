#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <kyu/kyu.h>

#define WIDTH 1024
#define HEIGHT 640

static GLuint vao;
static GLuint vbo;
static GLuint ebo;

static float colors[] = {
  1.f, 0.f, 0.f,
  0.f, 1.f, 0.f,
  0.f, 0.f, 1.f,
  0.f, 1.f, 0.f,
};

static GLuint program;
static kyu_mesh *mesh = NULL;

static void
init()
{
  const char* mesh_file = "data/quad.obj";
  size_t size, offset;

  clock_t before, after;
  double dur;

  kyu_matrix *mat = kyu_matrix_init(2, 3);
  mat->t[0] = 1.f;
  mat->t[1] = 0.f;
  mat->t[2] = 1.f;
  mat->t[1 * 3 + 0] = 0.f;
  mat->t[1 * 3 + 1] = 1.f;
  mat->t[1 * 3 + 2] = 1.f;
  kyu_matrix_print(mat);
  
  kyu_vec v = kyu_vec_init(2, 1, 3);
  kyu_matrix_mult_vec(mat, mat, &v);
  kyu_matrix_print(mat);
  kyu_matrix_release(mat);

  before = clock();
  mesh = kyu_mesh_read(mesh_file);
  after = clock();
  
  dur = 1000.0 * (after - before)/CLOCKS_PER_SEC;
  printf("\n-------------------------\nMesh '%s': %fms\n", mesh_file, dur);
  
  /* VAO and VBO */
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size = mesh->nb_vertices * sizeof(kyu_point) + sizeof(colors);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

  offset = 0;
  size = mesh->nb_vertices * sizeof(kyu_point);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh->vertices);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)offset);
  glEnableVertexAttribArray(0);

  offset += size;
  size = sizeof(colors);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, colors);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)offset);
  glEnableVertexAttribArray(1);

  size = sizeof(unsigned int) * 3 * mesh->nb_triangles;
  unsigned int *indices = malloc(size);
  for (int i = 0; i < mesh->nb_triangles; ++i)
    {
      const kyu_triangle *tri = &mesh->triangles[i];
      for (int j = 0; j < 3; ++j)
        indices[i * 3 + j] = tri->vertices[j];
    }
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  
  free(indices);

  glBindVertexArray(0);

  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  
  /* Shaders */
  program = read_shaders("shaders/base_vertex.glsl", "shaders/base_fragment.glsl");
}

static void
quit()
{
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);

  kyu_mesh_release(mesh);
}

static float angleY = 10.f;
static float angleZ = 0.f;

static void
render()
{
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  angleY += 1.f;
  angleZ += 0.5f;
  kyu_vec v = kyu_vec_init(0.5f, 0.f, 0.f);
  kyu_matrix *m = kyu_matrix_rotateZ(angleZ);
  kyu_matrix *m1 = kyu_matrix_rotateY(angleY);
  kyu_matrix_mult(m, m1, m);

  glUseProgram(program);

  GLint l = glGetUniformLocation(program, "mat");
  glUniformMatrix4fv(l, 1, GL_TRUE, m->t);
  
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, mesh->nb_triangles * 3, GL_UNSIGNED_INT, 0);

  kyu_matrix_release(m1);
  kyu_matrix_release(m);
}

int
main()
{  
  kyu_app *app = kyu_init(WIDTH, HEIGHT, "Kyu v" KYU_VERSION, init, quit, render);
  if (app == NULL)
    {
      fprintf(stderr, "Error while creating the app\n");
      exit(EXIT_FAILURE);
    }

  return kyu_run(app);
}
