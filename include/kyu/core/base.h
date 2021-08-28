#ifndef KYU_BASE_H
#define KYU_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kyu/graphics/gl.h"

typedef struct kyu_app kyu_app;

kyu_app *kyu_init(int width, int height, const char *name,
                  void (*init)(), void (*quit)(), void (*render)());
int kyu_run(kyu_app *app);
  
#ifdef __cplusplus
}
#endif

#endif /* KYU_BASE_H */
