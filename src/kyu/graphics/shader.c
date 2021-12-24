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

#include "kyu/core/utils.h"
#include "kyu/core/file.h"
#include "kyu/graphics/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFO_SIZE 512

static const char *VERTEX_SHADER = "Vertex";
static const char *FRAGMENT_SHADER = "Fragment";

GLuint
read_shaders(const char *restrict vertex_file,
             const char *restrict fragment_file)
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

int
create_shader(const char *filename, const GLuint type)
{
  int shader, success;
  kyu_file *file;
  char info[INFO_SIZE];
  char *current_type, *buffer;

  if (filename == NULL)
    return -1;
  
  if ((file = kyu_open_file(filename, "r")) == NULL)
    {
      KYU_LOG_ERROR("Failed to open the file \"%s\", exiting", filename);
      return -1;
    }

  kyu_mmap_file(&buffer, file);
  
  switch (type)
    {
    case GL_VERTEX_SHADER:
      current_type = (char*)VERTEX_SHADER;
      break;

    case GL_FRAGMENT_SHADER:
      current_type = (char*)FRAGMENT_SHADER;
      break;

    default:
      current_type = "\0";
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

  kyu_unmap_file(&buffer, file);
  kyu_close_file(file);
  
  return shader;
}
