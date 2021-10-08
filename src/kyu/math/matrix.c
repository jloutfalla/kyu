/* matrix -- simple matrix manipulation

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

#include "kyu/math/matrix.h"
#include "kyu/core/utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static kyu_vec get_matrix_column(kyu_matrix *matrix, int column);
static void set_matrix_column(kyu_matrix *matrix, kyu_vec *vec, int column);

kyu_matrix *
kyu_matrix_init(int height, int width)
{
  kyu_matrix *mat;

  mat = (kyu_matrix *)malloc(sizeof(kyu_matrix));
  KYU_ASSERT(mat != NULL, "Can't allocate memory for the matrix structure");
  if (mat == NULL)
    return mat;

  mat->width = width;
  mat->height = height;

  mat->t = (float *)malloc(height * width * sizeof(float));
  KYU_ASSERT(mat->t != NULL, "Can't allocate memory for the matrix tab");

  return mat;
}

kyu_matrix *
kyu_matrix_init4x4()
{
  return kyu_matrix_init(4, 4);
}

kyu_matrix *
kyu_matrix_init3x4()
{
  return kyu_matrix_init(3, 4);
}

kyu_matrix *
kyu_matrix_init4x3()
{
  return kyu_matrix_init(4, 3);
}

kyu_matrix *
kyu_matrix_identity(int height, int width)
{
  int i, j, index;
  kyu_matrix *mat;

  mat = kyu_matrix_init(height, width);
  if (mat == NULL)
    return NULL;

  for (i = 0; i < height; ++i)
    {
      for (j = 0; j < width; ++j)
        {
          index = i * width + j;
          mat->t[index] = 0.f;
          if (i == j)
            mat->t[index] = 1.f;
        }
    }
  
  return mat;
}

kyu_matrix *
kyu_matrix_identity4x4()
{
  return kyu_matrix_identity(4, 4);
}

kyu_matrix *
kyu_matrix_identity3x4()
{
  return kyu_matrix_identity(3, 4);
}

kyu_matrix *
kyu_matrix_identity4x3()
{
  return kyu_matrix_identity(4, 3);
}

int
kyu_matrix_release(kyu_matrix *matrix)
{
  KYU_ASSERT(matrix != NULL, "No matrix provided");
  if (matrix == NULL)
    return -1;

  KYU_ASSERT(matrix->t != NULL, "No memory allocated for the matrix tab");
  if (matrix->t == NULL)
    return -1;

  free(matrix->t);
  free(matrix);

  return 0;
}

void
kyu_matrix_transpose(kyu_matrix *dest, kyu_matrix *matrix)
{
  int i, j;
  float *tab;
  kyu_matrix *temp;

  KYU_ASSERT(dest != NULL, "No destination matrix provided");
  KYU_ASSERT(matrix != NULL, "No source matrix provided");
  if (dest == NULL || matrix == NULL)
    return;

  KYU_ASSERT(dest->height == matrix->width && dest->width == matrix->height,
             "Size of matrices doesn't match");
  if (dest->height != matrix->width || dest->width != matrix->height)
    return;
  
  temp = dest;
  if (dest == matrix)
    temp = kyu_matrix_init(matrix->width, matrix->height);

  for (i = 0; i < matrix->width; ++i)
    {
      for (j = 0; j < matrix->height; ++j)
        temp->t[i * matrix->height + j] = matrix->t[j * matrix->width + i];
    }

  tab = temp->t;
  temp->t = dest->t;
  dest->t = tab;

  if (dest == matrix)
    kyu_matrix_release(temp);
}

static kyu_vec
get_matrix_column(kyu_matrix *matrix, int column)
{
  kyu_vec ret = kyu_vec_init(0.f, 0.f, 0.f);
  
  KYU_ASSERT(matrix != NULL, "No matrix provided");
  if (matrix == NULL)
    return ret;
  
  KYU_ASSERT(matrix->width == 4 && matrix->height == 4,
             "Matrix width or height is not 4");
  if (matrix->width != 4 || matrix->height != 4)
    return ret;

  ret.x = matrix->t[column];
  ret.y = matrix->t[column + matrix->width];
  ret.z = matrix->t[column + 2 * matrix->width];
  ret.w = matrix->t[column + 3 * matrix->width];
  
  return ret;
}

static void
set_matrix_column(kyu_matrix *matrix, kyu_vec *vec, int column)
{
  KYU_ASSERT(matrix != NULL, "No matrix provided");
  KYU_ASSERT(vec != NULL, "No vector provided");
  if (matrix == NULL || vec == NULL)
    return;
  
  KYU_ASSERT(matrix->width == 4 && matrix->height == 4,
             "Matrix width or height is not 4");
  if (matrix->width != 4 || matrix->height != 4)
    return;

  matrix->t[column] = vec->x;
  matrix->t[column + matrix->width] = vec->y;
  matrix->t[column + 2 * matrix->width] = vec->z;
  matrix->t[column + 3 * matrix->width] = vec->w;
}

kyu_vec
kyu_matrix_getI(kyu_matrix *matrix)
{
  return get_matrix_column(matrix, 0);
}

kyu_vec
kyu_matrix_getJ(kyu_matrix *matrix)
{
  return get_matrix_column(matrix, 1);
}

kyu_vec
kyu_matrix_getK(kyu_matrix *matrix)
{
  return get_matrix_column(matrix, 2);
}

kyu_vec
kyu_matrix_getO(kyu_matrix *matrix)
{
  return get_matrix_column(matrix, 3);
}

void
kyu_matrix_setI(kyu_matrix *matrix, kyu_vec *vec)
{
  set_matrix_column(matrix, vec, 0);
}

void
kyu_matrix_setJ(kyu_matrix *matrix, kyu_vec *vec)
{
  set_matrix_column(matrix, vec, 1);
}

void
kyu_matrix_setK(kyu_matrix *matrix, kyu_vec *vec)
{
  set_matrix_column(matrix, vec, 2);
}

void
kyu_matrix_setO(kyu_matrix *matrix, kyu_vec *vec)
{
  set_matrix_column(matrix, vec, 3);
}

void
kyu_matrix_add(kyu_matrix *dest, kyu_matrix *a, kyu_matrix *b)
{
  int i, j, index;
  
  KYU_ASSERT(dest != NULL, "No destination matrix provided");
  KYU_ASSERT(a != NULL, "No left matrix provided");
  KYU_ASSERT(b != NULL, "No right matrix provided");
  if (dest == NULL || a == NULL || b == NULL)
    return;

  KYU_ASSERT(dest->width == a->width && dest->width == b->width,
             "Width of matrices doesn't match");
  KYU_ASSERT(dest->height == a->height && dest->height == b->height,
             "Height of matrices doesn't match");

  if (dest->width != a->width || dest->width != b->width
      || dest->height != a->height || dest->height != b->height)
    return;

  for (i = 0; i < dest->height; ++i)
    {
      for (j = 0; j < dest->width; ++j)
        {
          index = i * dest->width + j;
          dest->t[index] = a->t[index] + b->t[index];
        }
    }
}

void
kyu_matrix_sub(kyu_matrix *dest, kyu_matrix *a, kyu_matrix *b)
{
  int i, j, index;
  
  KYU_ASSERT(dest != NULL, "No destination matrix provided");
  KYU_ASSERT(a != NULL, "No left matrix provided");
  KYU_ASSERT(b != NULL, "No right matrix provided");
  if (dest == NULL || a == NULL || b == NULL)
    return;

  KYU_ASSERT(dest->width == a->width && dest->width == b->width,
             "Width of matrices doesn't match");
  KYU_ASSERT(dest->height == a->height && dest->height == b->height,
             "Height of matrices doesn't match");

  if (dest->width != a->width || dest->width != b->width
      || dest->height != a->height || dest->height != b->height)
    return;

  for (i = 0; i < dest->height; ++i)
    {
      for (j = 0; j < dest->width; ++j)
        {
          index = i * dest->width + j;
          dest->t[index] = a->t[index] - b->t[index];
        }
    }
}

void
kyu_matrix_mult(kyu_matrix *dest, kyu_matrix *a, kyu_matrix *b)
{
  int i, j, k;
  float s, *tab;
  kyu_matrix *temp = NULL;
  
  KYU_ASSERT(dest != NULL, "No destination matrix provided");
  KYU_ASSERT(a != NULL, "No left matrix provided");
  KYU_ASSERT(b != NULL, "No right matrix provided");
  if (dest == NULL || a == NULL || b == NULL)
    return;

  KYU_ASSERT(dest->height == a->height && a->width == b->height,
             "Size of matrices doesn't match");
  if (dest->height != a->height || a->width != b->height)
    return;

  temp = dest;
  if (dest == a || dest == b)
    temp = kyu_matrix_init(a->height, b->width);

  for (i = 0; i < temp->height; ++i)
    {
      for (j = 0; j < temp->width; ++j)
        {
          s = 0.f;
          for (k = 0; k < a->width; ++k)
            s += a->t[i * a->width + k] * b->t[k * b->width + j];

          temp->t[i * temp->width + j] = s;
        }
    }

  dest->height = a->height;
  dest->width = b->width;
  
  tab = temp->t;
  temp->t = dest->t;
  dest->t = tab;

  if (dest == a || dest == b)
    kyu_matrix_release(temp);
}

void
kyu_matrix_mult_vec(kyu_matrix *dest, kyu_matrix *a, kyu_vec *b)
{
  kyu_matrix *temp;

  KYU_ASSERT(dest != NULL, "No destination vector provided");
  KYU_ASSERT(a != NULL, "No left matrix provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (dest == NULL || a == NULL || b == NULL)
    return;

  KYU_ASSERT(a->width == 3 || a->width == 4,
             "Size of left matrix doesn't match");
  if (a->width != 3 && a->width != 4)
    return;
  
  temp = kyu_matrix_init(a->width, 1);
  temp->t[0] = b->x;
  temp->t[1] = b->y;
  temp->t[2] = b->z;
  if (a->width == 4)
    temp->t[3] = b->w;

  kyu_matrix_mult(dest, a, temp);

  kyu_matrix_release(temp);
}

void
kyu_matrix_mult_vec2(kyu_matrix *dest, kyu_matrix *a, kyu_vec *b)
{
  kyu_matrix *temp;

  KYU_ASSERT(dest != NULL, "No destination vector provided");
  KYU_ASSERT(a != NULL, "No left matrix provided");
  KYU_ASSERT(b != NULL, "No right vector provided");
  if (dest == NULL || a == NULL || b == NULL)
    return;

  KYU_ASSERT(a->width == 2, "Size of left matrix doesn't match");
  if (a->width != 2)
    return;
  
  temp = kyu_matrix_init(2, 1);
  temp->t[0] = b->x;
  temp->t[1] = b->y;

  kyu_matrix_mult(dest, a, temp);

  kyu_matrix_release(temp);
}

kyu_matrix *
kyu_matrix_translate(float x, float y, float z)
{
  kyu_vec v = kyu_point_init(x, y, z);
  return kyu_matrix_translate_vec(&v);
}

kyu_matrix *
kyu_matrix_translate_vec(kyu_vec *vec)
{
  kyu_matrix *ret = NULL;
  
  KYU_ASSERT(vec != NULL, "No vector provided");
  if (vec == NULL)
    return ret;

  ret = kyu_matrix_identity4x4();
  if (ret == NULL)
    return ret;

  kyu_matrix_setO(ret, vec);

  return ret;
}

kyu_matrix *
kyu_matrix_rotateX(float angle)
{
  kyu_vec a, b;
  kyu_matrix *ret = NULL;
  float rad = RADF(angle);
  
  ret = kyu_matrix_identity4x4();
  if (ret == NULL)
    return ret;

  a = kyu_vec_init(0.f,  cosf(rad), sinf(rad));
  b = kyu_vec_init(0.f, -sinf(rad), cosf(rad));

  kyu_matrix_setJ(ret, &a);
  kyu_matrix_setK(ret, &b);

  return ret;
}

kyu_matrix *
kyu_matrix_rotateY(float angle)
{
  kyu_vec a, b;
  kyu_matrix *ret = NULL;
  float rad = RADF(angle);
  
  ret = kyu_matrix_identity4x4();
  if (ret == NULL)
    return ret;
  
  a = kyu_vec_init(cosf(rad), 0.f, -sinf(rad));
  b = kyu_vec_init(sinf(rad), 0.f,  cosf(rad));

  kyu_matrix_setI(ret, &a);
  kyu_matrix_setK(ret, &b);

  return ret;
}

kyu_matrix *
kyu_matrix_rotateZ(float angle)
{
  kyu_vec a, b;
  kyu_matrix *ret = NULL;
  float rad = RADF(angle);
  
  ret = kyu_matrix_identity4x4();
  if (ret == NULL)
    return ret;

  a = kyu_vec_init( cosf(rad), sinf(rad), 0.f);
  b = kyu_vec_init(-sinf(rad), cosf(rad), 0.f);

  kyu_matrix_setI(ret, &a);
  kyu_matrix_setJ(ret, &b);

  return ret;
}

void
kyu_matrix_print(kyu_matrix *matrix)
{
  int i, j, index;

  KYU_ASSERT(matrix != NULL, "No matrix provided");
  if (matrix == NULL)
    return;

  printf("[");
  for(i = 0; i < matrix->height; ++i)
    {
      for (j = 0; j < matrix->width; ++j)
        {
          index = i * matrix->width + j;
          printf(" %.2f", matrix->t[index]);
        }

      if (i != matrix->height - 1)
        printf("\n");
      else
        printf(" ");
    }
  printf("]\n");
}
