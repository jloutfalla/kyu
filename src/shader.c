#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE(X) ((sizeof(X) / sizeof(X[0])))

#define BUFF_SIZE 4096

static const char *VERTEX_SHADER = "#ifdef VERTEX_SHADER";
static const char *FRAGMENT_SHADER = "#ifdef FRAGMENT_SHADER";
static const char *END_SHADER = "#endif";

GLuint read_shaders(const char* filename)
{
  GLuint prog, vertex_shader, fragment_shader;
  char buffer[BUFF_SIZE];

  /* Needs to be on heap */
  char *vertex_buff = (char *) malloc(BUFF_SIZE);
  char *fragment_buff = (char *) malloc(BUFF_SIZE);
  
  FILE* file;
  char *vertex, *vertex_end, *fragment, *fragment_end;
  unsigned int i;
  int success;
  char info[512];

  file = fopen(filename, "r");
  if (!file)
    {
      fprintf(stderr, "[Error] - %s", filename);
      perror(" ");
      exit(EXIT_FAILURE);
    }

  fread(buffer, sizeof(char), BUFF_SIZE, file);
  fclose(file);

  vertex = strstr(buffer, VERTEX_SHADER);
  fragment = strstr(buffer, FRAGMENT_SHADER);

  if (vertex)
    {
      for (i = 0; i < strlen(VERTEX_SHADER); ++i)
        vertex[i] = ' ';

      vertex_end = strstr(vertex, END_SHADER);
      if (vertex_end)
        vertex_end[0] = '\0';
    }

  if (fragment)
    {
      fragment = fragment + strlen(FRAGMENT_SHADER);
      fragment_end = strstr(fragment, END_SHADER);
      fragment_end[0] = '\0';
    }

  strcat(vertex_buff, buffer);
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1,
                 (const GLchar * const*) &vertex_buff, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(vertex_shader, SIZE(info), NULL, info);
      fprintf(stderr,
              "[GL ERROR] - Vertex shader compilation failed\n%s\n",
              info);
    }

  if (vertex)
    (buffer + (vertex - buffer))[0] = '\0';

  if (fragment)
    strcat(buffer, fragment);
  strcat(fragment_buff, buffer);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1,
                 (const GLchar * const*) &fragment_buff, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(fragment_shader, SIZE(info), NULL, info);
      fprintf(stderr,
              "[GL ERROR] - Fragment shader compilation failed\n%s\n",
              info);
    }

  free(vertex_buff);
  free(fragment_buff);
  
  prog = glCreateProgram();
  glAttachShader(prog, vertex_shader);
  glAttachShader(prog, fragment_shader);
  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success)
    {
      glGetProgramInfoLog(prog, SIZE(info), NULL, info);
      fprintf(stderr, "[GL ERROR] - Program linking failed\n%s\n", info);
    }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return prog;
}
