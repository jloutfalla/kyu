#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024
#define INFO_SIZE 512

static const char *VERTEX_SHADER = "Vertex";
static const char *FRAGMENT_SHADER = "Fragment";

GLuint read_shaders(const char* vertex_file, const char* fragment_file)
{
  GLuint prog;
  int vertex_shader, fragment_shader;
  int success;
  char info[INFO_SIZE];

  vertex_shader = create_shader(vertex_file, GL_VERTEX_SHADER);
  fragment_shader = create_shader(fragment_file, GL_FRAGMENT_SHADER);
  
  prog = glCreateProgram();

  if (vertex_shader > 0)
    glAttachShader(prog, vertex_shader);

  if (fragment_shader > 0)
    glAttachShader(prog, fragment_shader);
  
  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success)
    {
      glGetProgramInfoLog(prog, INFO_SIZE, NULL, info);
      fprintf(stderr, "[GL ERROR] - Program linking failed\n\t%s\n", info);
    }

  if (vertex_shader)
    glDeleteShader(vertex_shader);

  if (fragment_shader)
    glDeleteShader(fragment_shader);

  return prog;
}

int create_shader(const char* filename, const GLuint type)
{
  int shader, success, read;
  FILE *file;
  char info[INFO_SIZE];
  char current_type[9];

  /* Must be on heap */
  char *buffer;

  if (filename == NULL)
    return -1;
  
  file = fopen(filename, "r");
  if (!file)
    {
      fprintf(stderr, "[Error] - %s", filename);
      perror(" ");
      exit(EXIT_FAILURE);
    }

  buffer = (char *) malloc(BUFF_SIZE * sizeof(char));
  if (buffer == NULL)
    {
      fprintf(stderr, "[Error] - Don't have enough memory\n");
      exit(EXIT_FAILURE);
    }

  read = fread(buffer, sizeof(char), BUFF_SIZE, file);
  fclose(file);

  buffer[read] = '\0';
  
  switch (type)
    {
    case GL_VERTEX_SHADER:
      strncpy(current_type, VERTEX_SHADER, strlen(VERTEX_SHADER) + 1);
      break;

    case GL_FRAGMENT_SHADER:
      strncpy(current_type, FRAGMENT_SHADER, strlen(FRAGMENT_SHADER) + 1);
      break;
    }
  
  shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar * const*) &buffer, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(shader, INFO_SIZE, NULL, info);
      fprintf(stderr,
              "[GL ERROR] - %s shader compilation failed\n\t%s\n",
              current_type, info);
    }

  free(buffer);
  return shader;
}
