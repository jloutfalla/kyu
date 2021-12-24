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

#include "kyu/core/utils.h"

#include <stdio.h>
#include <stdarg.h>

#define KYU_LOG_MESSAGE_SIZE 256

void
kyu_log(kyu_log_type type, const char *restrict file, int line,
        const char *restrict message, ...)
{
  char *type_message, log_message[KYU_LOG_MESSAGE_SIZE];
  
  va_list args;
  va_start(args, message);

  vsnprintf(log_message, KYU_LOG_MESSAGE_SIZE - 1, message, args);

  log_message[KYU_LOG_MESSAGE_SIZE - 1] = '\0';

  va_end(args);

  switch (type)
    {
    case LOG:
      type_message = "[LOG]";
      break;
      
    case WARNING:
      type_message = BOLD YELLOW "[WARNING]" RESET YELLOW;
      break;

    case ERROR:
      type_message = BOLD RED "[ERROR]" RESET RED;
      break;

    case GL_LOG:
      type_message = "[GL LOG]";
      break;

    case GL_WARNING:
      type_message = BOLD YELLOW "[GL WARNING]" RESET YELLOW;
      break;
      
    case GL_ERROR:
      type_message = BOLD RED "[GL ERROR]" RESET RED;
      break;

    default:
      type_message = "\0";
      break;
    }

  if (file == NULL)
    fprintf(KYU_STDERR, "%s - %s" RESET "\n", type_message, log_message);
  else
    fprintf(KYU_STDERR, "%s:%d: %s - %s" RESET "\n", file, line, type_message, log_message);
}
