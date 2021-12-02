/* file -- file utilities

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

#include "kyu/core/utils.h"

#define FMMAP_LOG(...) KYU_ASSERT(0, __VA_ARGS__)
#define FMMAPDEF inline static
#define FMMAP_IMPLEMENTATION
#include "kyu/core/file.h"

kyu_file *
kyu_open_file(const char *restrict filename, const char *restrict mode)
{
  return fmmap_open_file(filename, mode);
}

int
kyu_close_file(kyu_file *file)
{
  return fmmap_close_file(file);
}

int
kyu_mmap_file(char **buff, const kyu_file *file)
{
  return fmmap_mmap_file(buff, file);
}

int
kyu_unmap_file(char **buff, const kyu_file *file)
{
  return fmmap_unmap_file(buff, file);
}
