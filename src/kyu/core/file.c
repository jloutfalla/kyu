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

#include "kyu/core/file.h"
#include "kyu/core/utils.h"

#if defined (__KYU_UNIX__)
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#elif defined (__KYU_WIN__)
#include <Windows.h>
#include <io.h>
#endif /* __KYU_WIN__ */

#include <stdlib.h>
#include <string.h>

struct os_spec {
  int fd;
#ifdef __KYU_WIN__
  HANDLE handle_file;
  HANDLE handle_map;
#endif /* __KYU_WIN__ */
};

int kyu_file_init_os(kyu_file *file);
int kyu_file_release_os(kyu_file *file);
int kyu_file_size(kyu_file *file);

kyu_file *
kyu_file_open(const char *restrict filename, const char *restrict mode)
{
  size_t length;
  FILE *f;
  kyu_file *file;
  
  KYU_ASSERT(filename != NULL && *filename != '\0', "No filename provided");
  KYU_ASSERT(mode != NULL && *mode != '\0', "No opening mode provided");
  if (filename == NULL || *filename == '\0' || mode == NULL || *mode == '\0')
    return NULL;
  
  if (strncmp(filename, "-", 2) == 0)
  {
    KYU_ASSERT(0, "Doesn't allow to read a file from STDIN");
    return NULL;
  }

  f = fopen(filename, mode);
  KYU_ASSERT(f != NULL, "Can't open the file \"%s\"", filename);
  if (f == NULL)
    return NULL;

  file = (kyu_file *)malloc(sizeof(kyu_file));
  
  length = strnlen(filename, FILENAME_MAX);

  file->stream = f;
  file->name = malloc(length);
  strncpy(file->name, filename, length);

  file->os_spec = NULL;
  kyu_file_init_os(file);

  file->size = 0;
  kyu_file_size(file);

  return file;
}

int
kyu_file_close(kyu_file *file)
{
  int ret;

  KYU_ASSERT(file != NULL, "No file structure provided");
  if (file == NULL)
    return -1;
  
  ret = fclose(file->stream);
  KYU_ASSERT(ret == 0, "Can't close the file \"%s\"", file->name);
  if (ret != 0)
    return ret;

  free(file->name);
  
  file->size = 0;
  
  ret = kyu_file_release_os(file);
  
  free(file);
  
  return ret;
}

int
kyu_file_init_os(kyu_file *file)
{
  int ret;
  kyu_os_spec *spec;

  KYU_ASSERT(file != NULL, "No file structure provided");
  if (file == NULL)
    return -1;
  
  KYU_ASSERT(file->os_spec == NULL, "File OS specific structure already allocated");
  if (file->os_spec != NULL)
    kyu_file_release_os(file);
  
  spec = (kyu_os_spec *)malloc(sizeof(kyu_os_spec));
  KYU_ASSERT(spec != NULL, "Can't allocate OS specific structure");
  if (spec == NULL)
    return -1;
      
  file->os_spec = spec;
  
#if defined (__KYU_UNIX__)
  spec->fd = fileno(file->stream);
  KYU_ASSERT(spec->fd != -1, "Can't retrieve the file descriptor");
  ret = (spec->fd == -1);
#elif defined(__KYU_WIN__)
  spec->fd = _fileno(file->stream);
  KYU_ASSERT(spec->fd != -1, "Can't retrieve the file descriptor");

  spec->handle_file = (HANDLE)_get_osfhandle(spec->fd);
  KYU_ASSERT(spec->handle_file != INVALID_HANDLE_VALUE, "Invalid file handler retrieved");
  ret = (spec->handle_file == INVALID_HANDLE_VALUE);

  spec->handle_map = INVALID_HANDLE_VALUE;
#endif /* __KYU_WIN__ */

  return ret;
}

int
kyu_file_release_os(kyu_file *file)
{
  int ret;
  kyu_os_spec *spec;

  KYU_ASSERT(file != NULL, "No file structure provided");
  if (file == NULL)
    return -1;

  KYU_ASSERT(file->os_spec != NULL, "No OS specific structure allocated");
  if (file->os_spec == NULL)
    return -1;

  spec = file->os_spec;
  ret = 0;
  
#if defined (__KYU_WIN__)
  ret = 0;
  if (spec->handle_map != INVALID_HANDLE_VALUE)
    {
      ret = CloseHandle(spec->handle_map);
      ret = (ret == 0);
    }

  spec->handle_file = INVALID_HANDLE_VALUE;
#endif /* __KYU_WIN__ */
  
  free(spec);
  file->os_spec = NULL;
  
  return ret;
}

int
kyu_file_size(kyu_file *file)
{
  int ret;
  kyu_os_spec *spec;
  
  KYU_ASSERT(file != NULL, "No file");
  if (file == NULL)
    return -1;

  spec = (kyu_os_spec *)file->os_spec;
  KYU_ASSERT(spec != NULL, "No OS specific structure allocated");
  if (spec == NULL)
    return -1;
  
#if defined(__KYU_UNIX__)
  struct stat stats;
  
  ret = fstat(spec->fd, &stats);
  KYU_ASSERT(ret == 0, "Can't retrieve file stats");

  if (ret == 0)
    file->size = stats.st_size;
#elif defined(__KYU_WIN__)
  ret = _filelength(spec->fd);
  KYU_ASSERT(ret != -1, "Can't retrieve the file size");

  if (ret != -1)
  {
    file->size = ret;
    ret = 0;
  }
#endif /* __KYU_WIN__ */
  
  return ret;
}

int
kyu_file_map_memory(char **buff, const kyu_file *file)
{
  char *tmp_buff = NULL;
  void *test_ptr = NULL;
  kyu_os_spec *spec;

  KYU_ASSERT(file != NULL, "No file structure provided");
  KYU_ASSERT(buff != NULL, "No buffer provided");
  if (file == NULL || buff == NULL)
    return -1;

  spec = (kyu_os_spec *)file->os_spec;
  KYU_ASSERT(spec != NULL, "No OS specific structure allocated");
  if (spec == NULL)
    return -1;
  
#if defined(__KYU_UNIX__)
  tmp_buff = mmap(NULL, file->size, PROT_READ, MAP_PRIVATE, spec->fd, 0);
  test_ptr = MAP_FAILED;
#elif defined(__KYU_WIN__)
  spec->handle_map = CreateFileMapping(spec->handle_file, NULL, PAGE_READONLY, 0, 0, NULL);
  KYU_ASSERT(spec->handle_map != INVALID_HANDLE_VALUE, "Invalid handler retrieved when creating memory mapping");
  if (spec->handle_map == INVALID_HANDLE_VALUE)
    return -1;
  
  tmp_buff = MapViewOfFile(spec->handle_map, FILE_MAP_READ, 0, 0, 0);
  test_ptr = NULL;
#endif
  
  KYU_ASSERT(tmp_buff != test_ptr, "Can't map the file into memory");
  if (tmp_buff == test_ptr)
    return -1;

  *buff = tmp_buff;
  
  return 0;
}

int
kyu_file_unmap_memory(char **buff, const kyu_file *file)
{
  int ret = 0;
  kyu_os_spec *spec;

  KYU_ASSERT(file != NULL, "No file structure provided");
  KYU_ASSERT(buff != NULL && *buff != NULL, "No buffer provided");
  if (file == NULL || buff == NULL || *buff == NULL)
    return -1;
  
  KYU_ASSERT(file->size > 0, "The size of the memory map file is invalid " \
                             "(filename: \"%s\", size: %lu)", file->name, file->size);
  if (file->size <= 0)
    return -1;

  spec = (kyu_os_spec *)file->os_spec;
  KYU_ASSERT(spec != NULL, "No OS specific structure allocated");
  if (spec == NULL)
    return -1;
  
#if defined(__KYU_UNIX__)
  ret = munmap(*buff, file->size);
#elif defined(__KYU_WIN__)
  ret = CloseHandle(spec->handle_map);
  if (ret != 0)
    {
      spec->handle_map = INVALID_HANDLE_VALUE;
      ret = 0;
    }
#endif /* __KYU_WIN__ */
  
  KYU_ASSERT(ret == 0, "Can't unmap memory file: \"%s\"", file->name);
  if (ret != 0)
    return -1;
  
  *buff = NULL;
  
  return ret;
}
