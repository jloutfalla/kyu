/* kyu -- simple app for 3D rendering with OpenGL
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

#include "kyu/kyu.h"

#include <stdio.h>
#include <stdlib.h>

#include "glfw_utility.h"


#define ERR_EXIT(STR)              \
  {                                \
    KYU_LOG_ERROR("%s\n", (STR));  \
    exit(EXIT_FAILURE);            \
  }


kyu_app
kyu_init(int width, int height, const char *name,
         void (*init)(), void (*quit)(), void (*render)())
{
  kyu_app app = {
    .window = NULL,
    .init = init,
    .quit = quit,
    .render = render
  };

  GLFWwindow* window = NULL;
  GLenum err;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE
  
  window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      ERR_EXIT("Can't create a window...");
    }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resize_callback);
  glfwSetKeyCallback(window, input_callback);
  
  err = glewInit();
  if (err != GLEW_OK)
    {
      glfwTerminate();
      ERR_EXIT("Can't initialize GLEW...");
    }  

  app.window = window;
  
  return app;
}

int
kyu_run(const kyu_app *app)
{
  app->init();
  
  while (!glfwWindowShouldClose(app->window))
    {
      app->render();

      /* Swap front and back buffers */
      glfwSwapBuffers(app->window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  app->quit();
  glfwTerminate();
  return EXIT_SUCCESS;
}
