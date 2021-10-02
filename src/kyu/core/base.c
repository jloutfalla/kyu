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

#include "kyu/core/base.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "kyu/core/utils.h"

#include "utils/glfw_utility.h"

#define ERR_EXIT(STR)              \
  {                                \
    KYU_LOG_ERROR("%s\n", (STR));  \
    exit(EXIT_FAILURE);            \
  }

struct kyu_app {
  GLFWwindow *window;
  
  void (*init)();
  void (*quit)();
  void (*update)();
  void (*render)();
};

#ifndef NDEBUG
static void kyu_glad_pre_callback(const char *name, void *funcptr, int len_args, ...);
static void kyu_glad_post_callback(const char *name, void *funcptr, int len_args, ...);
static void APIENTRY kyu_log_error_callback(GLenum source, GLenum type, GLuint id,
                                            GLenum severity, GLsizei length,
                                            const GLchar *message, const void *userParam);

static unsigned char suppress_glad_callback = 0;
#endif /* !NDEBUG */

double kyu_deltatime = 0.0;

kyu_app *
kyu_init(int width, int height, const char *name,
         void (*init)(), void (*quit)(), void (*update)(), void (*render)())
{
  GLFWwindow* window = NULL;
  kyu_app *app = malloc(sizeof(kyu_app));

  if (app == NULL)
    {
      KYU_LOG_ERROR("Can't allocate a kyu_app struct");
      return NULL;
    }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

#ifdef __APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif /* __APPLE */
  
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
  
  if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0)
    {
      glfwTerminate();
      ERR_EXIT("Can't initialize GLAD...");
    }

#ifndef NDEBUG
  glad_set_pre_callback(kyu_glad_pre_callback);
  glad_set_post_callback(kyu_glad_post_callback);
  
  if (glDebugMessageCallback)
    {
      suppress_glad_callback = 1;

      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(kyu_log_error_callback, NULL);
      GLuint unusedIds = 0;
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
                            0, &unusedIds, GL_TRUE);
    }
#endif /* !NDEBUG */

  app->window = window;
  app->init = init;
  app->quit = quit;
  app->update = update;
  app->render = render;
  
  return app;
}

int
kyu_run(kyu_app *app)
{
  static double limit = 1.0 / KYU_FRAMERATE;
  int updates, frames;
  double last_time, now_time, delta_time, timer;
  
  KYU_ASSERT(app != NULL, "Pointer to kyu_app is NULL");
  if (app == NULL)
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
  
  app->init();

  kyu_deltatime = delta_time = 0.0;
  timer = last_time = glfwGetTime();
  updates = frames = 0;
  
  while (!glfwWindowShouldClose(app->window))
    {
      now_time = glfwGetTime();
      kyu_deltatime = (now_time - last_time);
      delta_time += kyu_deltatime / limit;
      last_time = now_time;
      
      while (delta_time >= 1.0)
        {
          app->update();
          updates++;
          delta_time--;
        }
      
      app->render();
      
      /* Swap front and back buffers */
      glfwSwapBuffers(app->window);
      frames++;
      
      /* Poll for and process events */
      glfwPollEvents();
      
      if (glfwGetTime() - timer > 1.0)
        {
          timer++;
          printf("\rUpdates : %d | Frames : %d", updates, frames);
          fflush(stdout);
          updates = frames = 0;
        }
    }
  printf("\n");
  
  app->quit();
  glfwTerminate();

  free(app);
  app = NULL;
  
  return EXIT_SUCCESS;
}

#ifndef NDEBUG
static void
kyu_glad_pre_callback(const char *name, void *funcptr, int len_args, ...)
{
  (void)name;
  (void)funcptr;
  (void)len_args;

  if (suppress_glad_callback == 1)
    return;
  
  while (glad_glGetError() != GL_NO_ERROR);
}

static void
kyu_glad_post_callback(const char *name, void *funcptr, int len_args, ...)
{
  (void)funcptr;
  (void)len_args;

  char *error_type = "";
  GLenum error = glad_glGetError();

  if (suppress_glad_callback == 1)
    return;

  switch (error)
    {
    case GL_INVALID_ENUM:
      error_type = "GL_INVALID_ENUM";
      break;

    case GL_INVALID_VALUE:
      error_type = "GL_INVALID_VALUE";
      break;

    case GL_INVALID_OPERATION:
      error_type = "GL_INVALID_OPERATION";
      break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error_type = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;

    case GL_OUT_OF_MEMORY:
      error_type = "GL_OUT_OF_MEMORY";
      break;

    case GL_NO_ERROR:
      return;
    }

  kyu_log(GL_ERROR, NULL, -1, "%s in %s", error_type, name);
}

static void
APIENTRY kyu_log_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam)
{
  (void)source;
  (void)id;
  (void)length;
  (void)userParam;

  kyu_log_type log_type;
  char *severity_s;
  
  switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
      log_type = GL_ERROR;
      break;

    case GL_DEBUG_TYPE_OTHER:
      log_type = GL_LOG;
      break;

    default:
      log_type = GL_WARNING;
      break;
    }

  switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
      severity_s = "LOW";
      break;
      
    case GL_DEBUG_SEVERITY_MEDIUM:
      severity_s = "MEDIUM";
      break;
      
    case GL_DEBUG_SEVERITY_HIGH:
      severity_s = "HIGH";
      break;

    default:
      severity_s = "";
      break;
    }

  if (*severity_s == '\0')
    kyu_log(log_type, NULL, -1, "%s", message);
  else
    kyu_log(log_type, NULL, -1, "[%s] - %s", severity_s, message);
}
#endif /* !NDEBUG */
