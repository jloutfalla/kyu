/* matrix -- matrix structures and functions

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

#ifndef KYU_MATRIX_H
#define KYU_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "kyu/math/vector.h"
  
typedef struct {
  int width, height;
  float *t;
} kyu_matrix;

kyu_matrix *kyu_matrix_init(int height, int width);
kyu_matrix *kyu_matrix_init4x4();
kyu_matrix *kyu_matrix_init3x4();
kyu_matrix *kyu_matrix_init4x3();
kyu_matrix *kyu_matrix_identity(int height, int width);
kyu_matrix *kyu_matrix_identity4x4();
kyu_matrix *kyu_matrix_identity3x4();
kyu_matrix *kyu_matrix_identity4x3();

int kyu_matrix_release(kyu_matrix *matrix);

kyu_matrix *kyu_matrix_from_vec(kyu_vec *vec);
void kyu_matrix_transpose(kyu_matrix *dest, kyu_matrix *matrix);

kyu_vec kyu_matrix_getI(kyu_matrix *matrix);
kyu_vec kyu_matrix_getJ(kyu_matrix *matrix);
kyu_vec kyu_matrix_getK(kyu_matrix *matrix);
kyu_vec kyu_matrix_getO(kyu_matrix *matrix);

void kyu_matrix_setI(kyu_matrix *matrix, kyu_vec *vec);
void kyu_matrix_setJ(kyu_matrix *matrix, kyu_vec *vec);
void kyu_matrix_setK(kyu_matrix *matrix, kyu_vec *vec);
void kyu_matrix_setO(kyu_matrix *matrix, kyu_vec *vec);

void kyu_matrix_add(kyu_matrix *dest, kyu_matrix *a, kyu_matrix *b);
void kyu_matrix_sub(kyu_matrix *dest, kyu_matrix *a, kyu_matrix *b);
void kyu_matrix_mult(kyu_matrix *dest, kyu_matrix *a, kyu_matrix *b);
void kyu_matrix_mult_vec(kyu_matrix *dest, kyu_matrix *a, kyu_vec *b);
void kyu_matrix_mult_vec2(kyu_matrix *dest, kyu_matrix *a, kyu_vec *b);

kyu_matrix *kyu_matrix_translate(float x, float y, float z);
kyu_matrix *kyu_matrix_translate_vec(kyu_vec *vec);
kyu_matrix *kyu_matrix_rotateX(float angle);
kyu_matrix *kyu_matrix_rotateY(float angle);
kyu_matrix *kyu_matrix_rotateZ(float angle);

void kyu_matrix_print(kyu_matrix *matrix);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* KYU_MATRIX_H */
