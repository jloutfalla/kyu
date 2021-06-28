#ifndef KYU_BASE_H
#define KYU_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "kyu/version.h"

  typedef struct 
  {
    GLFWwindow *window;
  
    void (*init)();
    void (*quit)();
    void (*render)();
  } kyu_app;


  kyu_app
  kyu_init(int width, int height, const char *name,
           void (*init)(), void (*quit)(), void (*render)());

  int
  kyu_run(const kyu_app *app);
  
#ifdef __cplusplus
}
#endif

#endif /* KYU_BASE_H */
