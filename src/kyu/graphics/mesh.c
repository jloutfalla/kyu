/* mesh -- functions to manipulate meshes

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

#include "kyu/graphics/mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "kyu/core/utils.h"
#include "kyu/core/file.h"

#define LINE_LENGTH 1024

#define CMP(X, Y) if (strncmp((X), (Y), strlen((Y))) == 0)
#define CHECK_ARRAY(ARR, SIZE, CAPACITY)                                    \
  {                                                                         \
    if ((SIZE) >= (CAPACITY))                                               \
      {                                                                     \
        int new_capacity = (CAPACITY) * 2;                                  \
        (ARR) = realloc((ARR), new_capacity * sizeof((ARR)[0]));            \
        (CAPACITY) = new_capacity;                                          \
                                                                            \
        KYU_ASSERT((ARR) != NULL, "Failed to realloc memory for an array"); \
      }                                                                     \
  }
#define SHRINK_ARRAY(ARR, SIZE) ((ARR) = realloc((ARR), (SIZE) * sizeof((ARR)[0])))

static const char *wavefront_skip[] = {
  "#",
  "o",
  "g",
  "l",
  "s",
  "vp",
  "mtllib",
  "usemtl",
  NULL
};

#define WAVE_VERTEX             "v"
#define WAVE_VERTEX_UV          "vt"
#define WAVE_VERTEX_NORMAL      "vn"
#define WAVE_FACE               "f"

static int  need_skip_line(int *index, const char *restrict line);
static int  fill_vertex(kyu_vec *restrict vertex, const char *restrict line);
static int  fill_vertex_normal(kyu_vec *restrict normal, const char *restrict line);
static int  fill_vertex_uv(kyu_vec2 *restrict uv, const char *restrict line);
static void fill_triangle(kyu_triangle **triangles,
                          int *restrict nb_triangles, int *restrict triangles_capacity,
                          int nb_vertices, int nb_normals, int nb_texcoords,
                          const char *restrict line);

kyu_mesh *
kyu_mesh_read(const char *restrict filename)
{
  TODO("Add colors to mesh");
  
  kyu_file *file;
  kyu_mesh *mesh;
  int skip, index, ret;
  int vertices_capacity, normals_capacity, uvs_capacity;
  int /*colors_capacity,*/ triangles_capacity;
  char line[LINE_LENGTH] = { 0 };
  char *ptr;

  KYU_ASSERT(filename != NULL, "No filename provided");
  
  if ((file = kyu_file_open(filename, "r")) == NULL)
    return NULL;

  mesh = (kyu_mesh *)malloc(sizeof(kyu_mesh));

  mesh->vertices       = (kyu_point *)malloc(3 * sizeof(kyu_point));
  mesh->normals        = (kyu_vec *)malloc(3 * sizeof(kyu_vec));
  mesh->uvs            = (kyu_vec2 *)malloc(3 * sizeof(kyu_vec2));
  mesh->triangles      = (kyu_triangle *)malloc(sizeof(kyu_triangle));
  mesh->colors         = NULL;
  
  mesh->nb_vertices    = 0;
  mesh->nb_normals     = 0;
  mesh->nb_uvs         = 0;
  mesh->nb_triangles   = 0;
  mesh->nb_colors      = 0;

  vertices_capacity    = 3;
  normals_capacity     = 3;
  uvs_capacity         = 3;
  
  triangles_capacity   = 1;
  /* colors_capacity      = 0; */
  
  while (fgets(line, LINE_LENGTH, file->stream))
    {
      skip = need_skip_line(&index, line);

      if (skip != 0)
        continue;

      ptr = line + index;
      CMP(ptr, WAVE_VERTEX_NORMAL)
        {
          CHECK_ARRAY(mesh->normals, mesh->nb_normals, normals_capacity);
          kyu_vec *normal = &mesh->normals[mesh->nb_normals];
          normal->w = 0.f;

          ret = fill_vertex_normal(normal, line);
          if (ret == 0)
            mesh->nb_normals++;
        }
      else CMP(ptr, WAVE_VERTEX_UV)
        {
          CHECK_ARRAY(mesh->uvs, mesh->nb_uvs, uvs_capacity);
          kyu_vec2 *uv = &mesh->uvs[mesh->nb_uvs];

          ret = fill_vertex_uv(uv, line);
          if (ret == 0)
            mesh->nb_uvs++;
        }
      else CMP(ptr, WAVE_VERTEX)
        {
          CHECK_ARRAY(mesh->vertices, mesh->nb_vertices, vertices_capacity);
          kyu_point *vertex = &mesh->vertices[mesh->nb_vertices];
          vertex->w = 1.f;
          
          ret = fill_vertex(vertex, line);
          if (ret == 0)
            mesh->nb_vertices++;
        }
      else  CMP(ptr, WAVE_FACE)
        {          
          CHECK_ARRAY(mesh->triangles, mesh->nb_triangles, triangles_capacity);

          fill_triangle(&mesh->triangles, &mesh->nb_triangles, &triangles_capacity,
                        mesh->nb_vertices, mesh->nb_normals, mesh->nb_uvs, line);
        }
    }

  SHRINK_ARRAY(mesh->vertices,  mesh->nb_vertices);
  SHRINK_ARRAY(mesh->normals,   mesh->nb_normals);
  SHRINK_ARRAY(mesh->uvs,       mesh->nb_uvs);

  /* SHRINK_ARRAY(mesh->colors,    mesh->nb_colors); */
  SHRINK_ARRAY(mesh->triangles, mesh->nb_triangles);
  
  kyu_file_close(file);
  return mesh;
}

void
kyu_mesh_release(kyu_mesh *mesh)
{
  KYU_ASSERT(mesh != NULL, "No mesh provided");
  
  if (mesh != NULL)
    {
      free(mesh->vertices);
      free(mesh->normals);
      free(mesh->uvs);
      free(mesh->triangles);
      free(mesh->colors);
      free(mesh);
    }
}

static int
need_skip_line(int *index, const char *restrict line)
{
  int skip_idx;
  *index = 0;
  skip_idx = 0;
      
  while (line[*index] && isspace(line[*index]))
    *index += 1;

  if (line[*index] == '\0')
    return 1;
  
  while (wavefront_skip[skip_idx] != NULL)
    {
      if (strstr(line, wavefront_skip[skip_idx]) != NULL)
        return 1;
      
      skip_idx++;
    }
  
  return 0;
}

static int
fill_vertex(kyu_point *restrict vertex, const char *restrict line)
{
  int num;
  
  num = sscanf(line, WAVE_VERTEX" %f %f %f %f",
               &vertex->x, &vertex->y, &vertex->z, &vertex->w);

  return (num < 3);
}

static int
fill_vertex_normal(kyu_vec *restrict normal, const char *restrict line)
{
  int num;

  num = sscanf(line, WAVE_VERTEX_NORMAL" %f %f %f %f",
               &normal->x, &normal->y, &normal->z, &normal->w);

  return (num < 3);
}

static int
fill_vertex_uv(kyu_vec2 *restrict uv, const char *restrict line)
{
  int num;

  num = sscanf(line, WAVE_VERTEX_UV" %f %f",
               &uv->x, &uv->y);
  
  return (num != 2);
}

static void
fill_triangle(kyu_triangle **triangles,
              int *restrict nb_triangles, int *restrict triangles_capacity,
              int nb_vertices, int nb_normals, int nb_texcoords,
              const char *restrict line)
{
  int i, next, need_increment;
  int v[4] = { 0, 0, 0, 0 }; /* vertices indexes */
  int t[4] = { 0, 0, 0, 0 }; /* vertices texture coordinates */
  int n[4] = { 0, 0, 0, 0 }; /* vertices normals */
  kyu_triangle *tris = *triangles;

  i = 0;
  for (line = line + 1; ; line = line + next, ++i)
    {
      next = 0;
      
      if (sscanf(line, " %d/%d/%d %n", &v[i], &t[i], &n[i], &next) == 3)
        continue;
      else if (sscanf(line, " %d/%d %n", &v[i], &t[i], &next) == 2)
        continue;
      else if (sscanf(line, " %d//%d %n", &v[i], &n[i], &next) == 2)
        continue;
      else if (sscanf(line, " %d %n", &v[i], &next) == 1)
        continue;
      else if (next == 0) /* end of the line */
        break;
    }

  i = 2;
  do
    {
      CHECK_ARRAY(tris, *nb_triangles, *triangles_capacity);
      *triangles = tris;
      
      kyu_triangle *tri = &tris[*nb_triangles];
      {
        for (int j = 0; j < 3; ++j)
          {
            tri->normals[j] = -1;
            tri->uvs[j] = -1;
          }
      }
      
      int idv[] = { 0, i - 1, i };

      need_increment = 1;
      for (int j = 0; j < 3; ++j)
        {
          int k = idv[j];
          int vert = (v[k] < 0) ? nb_vertices  + v[k] : v[k] - 1;
          int tex  = (t[k] < 0) ? nb_texcoords + t[k] : t[k] - 1;
          int norm = (n[k] < 0) ? nb_normals   + n[k] : n[k] - 1;

          if (vert < 0)
            {
              need_increment = 0;
              break; 
            }

          if (tex >= 0)
            tri->uvs[j] = tex;

          if (norm >= 0)
            tri->normals[j] = norm;
          
          tri->vertices[j] = vert;
        }

      *nb_triangles += need_increment;
      
      ++i;
    } while (i < 4 && v[i] != 0);
}
