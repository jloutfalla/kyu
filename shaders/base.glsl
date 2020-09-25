#version 330

#ifdef VERTEX_SHADER
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 f_color;

void main()
{
  f_color = color;
  gl_Position = vec4(position, 1.f);
}
#endif

#ifdef FRAGMENT_SHADER
in vec3 f_color;

out vec4 frag_color;

void main()
{
  frag_color = vec4(f_color, 1.f);
}
#endif
