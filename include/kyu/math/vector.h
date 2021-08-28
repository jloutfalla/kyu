/* vector -- vectors structures and functions

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

#ifndef KYU_VECTOR_H
#define KYU_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float x;
  float y;
  float z;
  float w;
} kyu_vec;
  
typedef struct {
  float x;
  float y;
} kyu_vec2;

typedef struct {
  float u;
  float v;
} kyu_uv;
  
typedef struct {
  float r;
  float g;
  float b;
  float a;
} kyu_color;

typedef kyu_vec kyu_point;

kyu_vec2  kyu_vec2_init(float x, float y);
kyu_uv    kyu_uv_init(float u, float v);
kyu_vec   kyu_vec_init(float x, float y, float z);
kyu_point kyu_point_init(float x, float y, float z);
kyu_color kyu_color_init(float r, float g, float b, float a);

kyu_vec kyu_vec_add(kyu_vec *a, kyu_vec *b);
kyu_vec kyu_vec_sub(kyu_vec *a, kyu_vec *b);
kyu_vec kyu_vec_mult(float a, kyu_vec *b);
kyu_vec kyu_vec_div(kyu_vec *a, float b);
  
kyu_point kyu_point_add(kyu_point *a, kyu_point *b);
kyu_point kyu_point_sub(kyu_point *a, kyu_point *b);
kyu_point kyu_point_mult(float a, kyu_point *b);
kyu_point kyu_point_div(kyu_point *a, float b);
  
kyu_vec2 kyu_vec2_add(kyu_vec2 *a, kyu_vec2 *b);
kyu_vec2 kyu_vec2_sub(kyu_vec2 *a, kyu_vec2 *b);
kyu_vec2 kyu_vec2_mult(float a, kyu_vec2 *b);
kyu_vec2 kyu_vec2_div(kyu_vec2 *a, float b);

kyu_vec normalize(kyu_vec *v);
kyu_vec2 normalize_2(kyu_vec2 *v);
kyu_point center(kyu_point *a, kyu_point *b);
float length(kyu_vec *v);
float length2(kyu_vec *v);
float length_2(kyu_vec2 *v);
float length2_2(kyu_vec2 *v);
  
float dot(kyu_vec *a, kyu_vec *b);
float dot2(kyu_vec2 *a, kyu_vec2 *b);
kyu_vec cross(kyu_vec *a, kyu_vec *b);

#ifdef __cplusplus
}
#endif

#endif /* KYU_VECTOR_H */
