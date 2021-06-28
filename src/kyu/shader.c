/* shader -- functions to read GLSL shaders

   Copyright (C) 2020, 2021 Jean-Baptiste Loutfalla <jb.loutfalla@orange.fr>

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

#include "kyu/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kyu/utils.h"

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
      KYU_LOG(GL_ERROR, "Program linking failed\n\t%s\n", info);
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
      KYU_LOG_ERROR("Can't open the file \"%s\"", filename);
      exit(EXIT_FAILURE);
    }

  buffer = (char *) malloc(BUFF_SIZE * sizeof(char));
  if (buffer == NULL)
    {
      KYU_LOG_ERROR("Don't have enough memory");
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
      KYU_LOG(GL_ERROR,
              "\"%s\" shader compilation failed\n\t%s",
              current_type, info);
    }

  free(buffer);
  return shader;
}
