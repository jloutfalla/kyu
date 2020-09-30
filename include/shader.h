#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

GLuint read_shaders(const char* vertex_file, const char* fragment_file);
int create_shader(const char* filename, const GLuint type);

#endif /* SHADER_H */
