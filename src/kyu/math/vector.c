/* vector -- vectors utility functions

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

#include "kyu/math/vector.h"
#include "kyu/core/utils.h"

#include <stdlib.h>
#include <math.h>

kyu_vec2
kyu_vec2_init(float x, float y)
{
  kyu_vec2 ret = {
    .x = x,
    .y = y
  };
    
  return ret;
}

kyu_uv
kyu_uv_init(float u, float v)
{
  kyu_uv ret = {
    .u = u,
    .v = v
  };

  return ret;
}

kyu_vec
kyu_vec_init(float x, float y, float z)
{
  kyu_vec ret = {
    .x = x,
    .y = y,
    .z = z,
    .w = 0.f
  };
    
  return ret;
}

kyu_point
kyu_point_init(float x, float y, float z)
{
  kyu_point ret = {
    .x = x,
    .y = y,
    .z = z,
    .w = 1.f
  };

  return ret;
}

kyu_color
kyu_color_init(float r, float g, float b, float a)
{
  kyu_color ret = {
    .r = r,
    .g = g,
    .b = b,
    .a = a
  };

  return ret;
}

kyu_vec
kyu_vec_add(kyu_vec *a, kyu_vec *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return kyu_vec_init(0.f, 0.f, 0.f);
  
  return kyu_vec_init(a->x + b->x, a->y + b->y, a->z + b->z);
}

kyu_vec
kyu_vec_sub(kyu_vec *a, kyu_vec *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return kyu_vec_init(0.f, 0.f, 0.f);
  
  return kyu_vec_init(a->x - b->x, a->y - b->y, a->z - b->z);
}

kyu_vec
kyu_vec_mult(float a, kyu_vec *b)
{
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (b == NULL)
    return kyu_vec_init(0.f, 0.f, 0.f);
  
  return kyu_vec_init(a * b->x, a * b->y, a * b->z);
}

kyu_vec
kyu_vec_div(kyu_vec *a, float b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  if (a == NULL)
    return kyu_vec_init(0.f, 0.f, 0.f);

  return kyu_vec_init(a->x / b, a->y / b, a->z / b);
}

kyu_point
kyu_point_add(kyu_point *a, kyu_point *b)
{
  KYU_ASSERT(a != NULL, "No left point provided");
  KYU_ASSERT(b != NULL, "No right point provided");
  if (a == NULL || b == NULL)
    return kyu_point_init(0.f, 0.f, 0.f);

  return kyu_vec_init(a->x + b->x, a->y + b->y, a->z + b->z);
}

kyu_point
kyu_point_sub(kyu_point *a, kyu_point *b)
{
  KYU_ASSERT(a != NULL, "No left point provided");
  KYU_ASSERT(b != NULL, "No right point provided");
  if (a == NULL || b == NULL)
    return kyu_point_init(0.f, 0.f, 0.f);

  return kyu_vec_init(a->x - b->x, a->y - b->y, a->z - b->z);
}

kyu_point
kyu_point_mult(float a, kyu_point *b)
{
  KYU_ASSERT(b != NULL, "No right point provided");
  if (b == NULL)
    return kyu_point_init(0.f, 0.f, 0.f);
  
  return kyu_point_init(a * b->x, a * b->y, a * b->z);
}

kyu_point
kyu_point_div(kyu_point *a, float b)
{
  KYU_ASSERT(a != NULL, "No left point provided");
  if (a == NULL)
    return kyu_point_init(0.f, 0.f, 0.f);

  return kyu_point_init(a->x / b, a->y / b, a->z / b);
}

kyu_vec2
kyu_vec2_add(kyu_vec2 *a, kyu_vec2 *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return kyu_vec2_init(0.f, 0.f);
  
  return kyu_vec2_init(a->x + b->x, a->y + b->y);
}

kyu_vec2
kyu_vec2_sub(kyu_vec2 *a, kyu_vec2 *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return kyu_vec2_init(0.f, 0.f);
  
  return kyu_vec2_init(a->x - b->x, a->y - b->y);
}

kyu_vec2
kyu_vec2_mult(float a, kyu_vec2 *b)
{
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (b == NULL)
    return kyu_vec2_init(0.f, 0.f);
  
  return kyu_vec2_init(a * b->x, a * b->y);
}

kyu_vec2
kyu_vec2_div(kyu_vec2 *a, float b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  if (a == NULL)
    return kyu_vec2_init(0.f, 0.f);

  return kyu_vec2_init(a->x / b, a->y / b);
}

kyu_vec
normalize(kyu_vec *v)
{
  float l;
  
  KYU_ASSERT(v != NULL, "No vector provided");
  if (v == NULL)
    return kyu_vec_init(0.f, 0.f, 0.f);
  
  l = 1.f / length(v);
  return kyu_vec_mult(l, v);
}

kyu_vec2
normalize_2(kyu_vec2 *v)
{
  float l;
  
  KYU_ASSERT(v != NULL, "No vector provided");
  if (v == NULL)
    return kyu_vec2_init(0.f, 0.f);
  
  l = 1.f / length_2(v);
  return kyu_vec2_mult(l, v);
}

kyu_point
center(kyu_point *a, kyu_point *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return kyu_point_init(0.f, 0.f, 0.f);
  
  return kyu_point_init((a->x + b->x) / 2.f,
                        (a->y + b->y) / 2.f,
                        (a->z + b->z) / 2.f);
}

float
length(kyu_vec *v)
{
  return sqrt(length2(v));
}

float
length2(kyu_vec *v)
{
  KYU_ASSERT(v != NULL, "No vector provided");
  if (v == NULL)
    return -1.f;
  
  return (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
}

float
length_2(kyu_vec2 *v)
{
  return sqrt(length2_2(v));
}

float
length2_2(kyu_vec2 *v)
{
  KYU_ASSERT(v != NULL, "No vector provided");
  if (v == NULL)
    return -1.f;
  
  return (v->x * v->x) + (v->y * v->y);
}

float
dot(kyu_vec *a, kyu_vec *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return 0.f;

  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

float
dot2(kyu_vec2 *a, kyu_vec2 *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return 0.f;
  
  return (a->x * b->x) + (a->y * b->y);
}

kyu_vec
cross(kyu_vec *a, kyu_vec *b)
{
  KYU_ASSERT(a != NULL, "No left vector provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (a == NULL || b == NULL)
    return kyu_vec_init(0.f, 0.f, 0.f);

  return kyu_vec_init((a->y * b->z) - (a->z * b->y),
                      (a->z * b->x) - (a->x * b->z),
                      (a->x * b->y) - (a->y * b->x));
}
