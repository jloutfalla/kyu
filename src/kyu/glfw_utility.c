/* glfw_utility -- simple callbacks to be use with GLFW
   Copyright (C) 2020 Jean-Baptiste Loutfalla

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "glfw_utility.h"

void resize_callback(GLFWwindow *w, int width, int height)
{
  (void)w;
  glViewport(0, 0, width, height);
}

void input_callback(GLFWwindow *w, int key, int scancode, int action, int mode)
{
  (void)scancode;
  (void)mode;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(w, GLFW_TRUE);
}
