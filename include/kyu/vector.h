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
    union {
      float x;
      float u;
    };

    union {
      float y;
      float v;
    };
  } kyu_vec2;

  typedef struct {
    float r;
    float g;
    float b;
    float a;
  } kyu_color;

  typedef kyu_vec kyu_point;

  kyu_vec2  kyu_vec2_init(float x, float y);
  kyu_vec   kyu_vec_init(float x, float y, float z);
  kyu_point kyu_point_init(float x, float y, float z);
  kyu_color kyu_color_init(float r, float g, float b, float a);

#ifdef __cplusplus
}
#endif

#endif /* KYU_VECTOR_H */
