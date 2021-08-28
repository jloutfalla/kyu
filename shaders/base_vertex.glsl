#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;

uniform mat4 mat;

out vec3 f_color;

void main()
{
  f_color = color;
  gl_Position = mat * position;
}
