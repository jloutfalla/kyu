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
#endif

#define DO_PRAGMA(X) _Pragma(#X)
#define TODO(X) DO_PRAGMA(message("TODO - " #X))

  typedef enum {
    LOG,
    WARNING,
    ERROR,
    GL_ERROR,
    COUNT
  } kyu_log_type;

  void kyu_log(kyu_log_type type, const char *file, int line, const char *message, ...);

#define _KYU_LOG(X, ...) kyu_log(X, __FILE__, __LINE__, __VA_ARGS__)
#define KYU_LOG(TYPE, ...) _KYU_LOG(TYPE, __VA_ARGS__)
#define KYU_LOG_WARNING(...) _KYU_LOG(WARNING, __VA_ARGS__)
#define KYU_LOG_ERROR(...) _KYU_LOG(ERROR, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* KYU_UTILS_H */
