#ifndef KYU_BASE_H
#define KYU_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kyu/core/utils.h"
#ifndef __KYU_PS2__
#include "kyu/graphics/gl.h"
#define KYU_FRAMERATE 60.0
#else
  #include <tamtypes.h>
  
#define KYU_BUFFER_SIZE 100
  extern qword_t *g_buff;
  
  extern float g_screen_cx;
  extern float g_screen_cy;
  extern float g_screen_x;
  extern float g_screen_y;

#define KYU_FRAMERATE 50.0
#endif

  typedef struct kyu_app kyu_app;

  extern double kyu_deltatime;

  kyu_app *kyu_init(int width, int height, const char *name,
                    void (*init)(), void (*quit)(), void (*update)(), void *(*render)(void *v));
  int kyu_run(kyu_app *app);
  
#ifdef __cplusplus
}
#endif

#endif /* KYU_BASE_H */
