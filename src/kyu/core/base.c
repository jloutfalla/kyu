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

#ifndef __KYU_PS2__
#  include <GLFW/glfw3.h>
#  include "utils/glfw_utility.h"
#else
#  include <graph.h>
#  include <dma.h>
#  include <gs_psm.h>
#  include <gif_tags.h>
#  include <draw.h>

qword_t *g_buff = NULL;

float g_screen_cx = 0.f;
float g_screen_cy = 0.f;
float g_screen_x  = 0.f;
float g_screen_y  = 0.f;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_EXIT(STR)              \
  {                                \
    KYU_LOG_ERROR("%s\n", (STR));  \
    exit(EXIT_FAILURE);            \
  }

struct kyu_app {
#ifndef __KYU_PS2__
  GLFWwindow *window;
#else
  framebuffer_t fb;
  zbuffer_t z;
#endif
  
  void (*init)();
  void (*quit)();
  void (*update)();
  void *(*render)(void *);
};

#ifndef __KYU_PS2__
#ifndef NDEBUG
static void kyu_glad_pre_callback(const char *name, void *funcptr, int len_args, ...);
static void kyu_glad_post_callback(const char *name, void *funcptr, int len_args, ...);
static void APIENTRY kyu_log_error_callback(GLenum source, GLenum type, GLuint id,
                                            GLenum severity, GLsizei length,
                                            const GLchar *message, const void *userParam);

static unsigned char suppress_glad_callback = 0;
#endif /* !NDEBUG */
#endif /* !__KYU_PS2__ */

double kyu_deltatime = 0.0;

kyu_app *
kyu_init(int width, int height, const char *name,
         void (*init)(), void (*quit)(), void (*update)(), void *(*render)(void *))
{
  (void)name;
  kyu_app *app = malloc(sizeof(kyu_app));

  if (app == NULL)
    {
      KYU_LOG_ERROR("Can't allocate a kyu_app struct");
      return NULL;
    }
  
#ifndef __KYU_PS2__
  GLFWwindow* window = NULL;

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
#else /* __KYU_PS2__ */
  qword_t *q = NULL;
  framebuffer_t fb = { 0 };
  zbuffer_t z = { 0 };

  dma_channel_initialize(DMA_CHANNEL_GIF, NULL, 0);
  dma_channel_fast_waits(DMA_CHANNEL_GIF);
  
  g_screen_cx = (float)width  / 2.f;
  g_screen_cy = (float)height / 2.f;
  g_screen_x  = 2048.f - g_screen_cx;
  g_screen_y  = 2048.f - g_screen_cy;
  
  fb.address = graph_vram_allocate(width, height, GS_PSM_32, GRAPH_ALIGN_PAGE);
  fb.width   = width;
  fb.height  = height;
  fb.psm     = GS_PSM_32;
  fb.mask    = 0;

  z.enable = DRAW_ENABLE;

  graph_set_mode(GRAPH_MODE_NONINTERLACED, GRAPH_MODE_PAL, GRAPH_MODE_FIELD, GRAPH_DISABLE);
  graph_set_screen(0, 0, width, height);
  graph_set_bgcolor(0, 0, 0);
  graph_set_framebuffer_filtered(fb.address, width, fb.psm, 0, 0);
  graph_enable_output();

  if (g_buff == NULL)
    g_buff = malloc(sizeof(qword_t) * KYU_BUFFER_SIZE);
  
  memset(g_buff, 0, KYU_BUFFER_SIZE);
  q = g_buff;
  q = draw_setup_environment(q, 0, &fb, &z);
  q = draw_primitive_xyoffset(q, 0, g_screen_x, g_screen_y);
  q = draw_finish(q);
  dma_channel_send_normal(DMA_CHANNEL_GIF, g_buff, q - g_buff, 0, 0);
  dma_wait_fast();
  
  app->fb = fb;
  app->z  = z;
#endif /* __KYU_PS2__ */
  
  app->init   = init;
  app->quit   = quit;
  app->update = update;
  app->render = render;
  
  return app;
}

int
kyu_run(kyu_app *app)
{
  void *v = NULL;
  
  KYU_ASSERT(app != NULL, "Pointer to kyu_app is NULL");
  if (app == NULL)
    {
#ifndef __KYU_PS2__
      glfwTerminate();
#endif
      exit(EXIT_FAILURE);
    }

  if (app->init != NULL)
    app->init();

#ifndef __KYU_PS2__
  static double limit = 1.0 / KYU_FRAMERATE;
  int updates, frames;
  double last_time, now_time, delta_time, timer;

  kyu_deltatime = delta_time = 0.0;
  timer = last_time = glfwGetTime();
  updates = frames = 0;

  while (!glfwWindowShouldClose(app->window))
    {
      now_time = glfwGetTime();
      kyu_deltatime = (now_time - last_time);
      delta_time += kyu_deltatime / limit;
      last_time = now_time;

      while (app->update != NULL && delta_time >= 1.0)
        {
          app->update();
          updates++;
          delta_time--;
        }
#else /* __KYU_PS2__ */
  qword_t *q = NULL;
  while (1)
    {
      dma_wait_fast();
      memset(g_buff, 0, KYU_BUFFER_SIZE);
      q = g_buff;
      q = draw_clear(q, 0, g_screen_x, g_screen_y,
                     app->fb.width, app->fb.height,
                     20, 20, 20);

      if (app->update != NULL)
        app->update();

      v = q;
#endif
      
      v = app->render(v);

#ifndef __KYU_PS2__
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
#else /* __KYU_PS2__ */
      q = (qword_t *)v;
      q = draw_finish(q);
      dma_channel_send_normal(DMA_CHANNEL_GIF, g_buff, q - g_buff, 0, 0);
      draw_wait_finish();
      graph_wait_vsync();
#endif
    }
  printf("\n");

  if (app->quit != NULL)
    app->quit();

#ifndef __KYU_PS2__
  glfwTerminate();
#endif

  free(app);
  app = NULL;
  
  return EXIT_SUCCESS;
}

#ifndef __KYU_PS2__
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
#endif /* !__KYU_PS2__ */
