/* utils -- utility functions and macros for the kyu library

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

#ifndef KYU_UTILS_H
#define KYU_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <math.h>
#include <signal.h>

#if defined (__unix__) || defined (__unix) || (defined(__APPLE__) && defined (__MACH__))
#ifndef __KYU_UNIX__
#define __KYU_UNIX__
#endif /* __KYU_UNIX__ */
#endif /* __unix__ || __unix || (__APPLE__ && __MACH__) */

#if defined(__WIN32) || defined(__WIN64)
#ifndef __KYU_WIN__
#define __KYU_WIN__
#endif /* __KYU_WIN__ */
#endif /* __WIN32 || __WIN64 */

#if defined(__KYU_PS2__)
#undef __KYU_UNIX__
#undef __KYU_WIN__
#undef __KYU_PS2__
#define __KYU_PS2__
#endif

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
  
#define DO_PRAGMA(X) _Pragma(#X)
#define TODO(X) DO_PRAGMA(message("TODO - " #X))
  
#define TO_STRING(X) #X
#define MAX(A, B) ((A) < (B) ? (B) : (A))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

#define RAD(X) ((X) * M_PI / 180.0)
#define RADF(X) (float)RAD(X)

#define KYU_SHIFT_UINT64(x, n) (((uint64_t)x) << (n))

#ifdef __KYU_PS2__
  
#define KYU_STDERR stdout

  /* ANSI escape sequences */
#define ESC     ""
#define RESET   ESC

#define BOLD    ESC
  
#define RED     ESC
#define GREEN   ESC
#define YELLOW  ESC
  
#else
  
#define KYU_STDERR stderr

  /* ANSI escape sequences */

#define ESC     "\033"
#define RESET   ESC"[0m"

#ifdef __KYU_WIN__
#define BOLD
#else
#define BOLD    ESC"[1m"
#endif /* __KYU_WIN__ */

#define RED     ESC"[31m"
#define GREEN   ESC"[32m"
#define YELLOW  ESC"[33m"

#endif
  
  /*************************/

  typedef enum {
    LOG,
    WARNING,
    ERROR,
    GL_LOG,
    GL_WARNING,
    GL_ERROR,
    COUNT
  } kyu_log_type;

  void kyu_log(kyu_log_type type, const char *restrict file, int line,
               const char *restrict message, ...);

#define _KYU_LOG(X, ...) kyu_log(X, __FILE__, __LINE__, __VA_ARGS__)
#define KYU_LOG(TYPE, ...) _KYU_LOG(TYPE, __VA_ARGS__)
#define KYU_LOG_WARNING(...) _KYU_LOG(WARNING, __VA_ARGS__)
#define KYU_LOG_ERROR(...) _KYU_LOG(ERROR, __VA_ARGS__)
#define KYU_LOG_GL(SOURCE, TYPE, SEVERITY, ...) kyu_log(GL_LOG, __VA_ARGS)
#define KYU_LOG_GL_ERROR(...) _KYU_LOG(GL_ERROR, __VA_ARGS__)

#ifndef NDEBUG
#if defined __KYU_UNIX__ || defined __KYU_PS2__
#define KYU_BREAK raise(SIGTRAP)
#elif defined __KYU_WIN__
#define KYU_BREAK __debugbreak()
#endif
#else
#define KYU_BREAK (void)NULL
#endif

#define KYU_ASSERT(COND, ...)       \
  {                                 \
    if ((COND) == 0)                \
      {                             \
        KYU_LOG_ERROR(__VA_ARGS__); \
        KYU_BREAK;                  \
      }                             \
  }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* KYU_UTILS_H */
