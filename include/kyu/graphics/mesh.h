/* mesh -- meshes utility

   Copyright (C) 2021 Jean-Baptiste Loutfalla <jb.loutfalla@orange.fr>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#ifndef KYU_OBJ_H
#define KYU_OBJ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kyu/math/vector.h"

typedef struct {
  int vertices[3];
  int normals[3];
  int uvs[3];
} kyu_triangle;

typedef struct {
  kyu_point     *vertices;
  kyu_vec       *normals;
  kyu_vec2      *uvs;
  kyu_triangle  *triangles;
  kyu_color     *colors;

  int nb_vertices;
  int nb_normals;
  int nb_uvs;
  int nb_triangles;
  int nb_colors;
} kyu_mesh;

kyu_mesh *kyu_mesh_read(const char *restrict filename);
void kyu_mesh_release(kyu_mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif /* KYU_OBJ_H */
