#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 f_color;

void main()
{
  f_color = color;
  gl_Position = vec4(position, 1.f);
}
