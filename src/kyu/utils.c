/* kyu_utils -- utility functions and macros

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

#include "kyu/utils.h"

#include <stdio.h>
#include <stdarg.h>

#define KYU_LOG_MESSAGE_SIZE 256
#define TYPE_SIZE 11

void
kyu_log(kyu_log_type type, const char *file, int line, const char *message, ...)
{
  char log_message[KYU_LOG_MESSAGE_SIZE];
  char type_message[TYPE_SIZE];
  
  va_list args;
  va_start(args, message);

  vsnprintf(log_message, KYU_LOG_MESSAGE_SIZE - 1, message, args);

  log_message[KYU_LOG_MESSAGE_SIZE - 1] = '\0';

  va_end(args);

  switch (type)
    {
    case LOG:
      snprintf(type_message, TYPE_SIZE, "[LOG]");
      break;
      
    case WARNING:
      snprintf(type_message, TYPE_SIZE, "[WARNING]");
      break;

    case ERROR:
      snprintf(type_message, TYPE_SIZE, "[ERROR]");
      break;

    case GL_ERROR:
      snprintf(type_message, TYPE_SIZE, "[GL ERROR]");
      break;

    default:
      type_message[0] = '\0';
      break;
    }
  
  fprintf(stderr, "%s:%d: %s %s\n", file, line, type_message, log_message);
}
