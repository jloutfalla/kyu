/* shader -- utility to read GLSL shaders

   Copyright (C) 2020, 2021 Jean-Baptiste Loutfalla

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

#ifndef SHADER_H
#define SHADER_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "kyu/graphics/gl.h"
  
GLuint read_shaders(const char *restrict vertex_file,
                    const char *restrict fragment_file);
int create_shader(const char *filename, const GLuint type);

#ifdef __cplusplus
}
#endif
  
#endif /* SHADER_H */
