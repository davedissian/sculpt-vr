#version 330 core

layout(location = 0) in vec3 in_vertex;

uniform mat4 u_view;
uniform mat4 u_proj;

void main(void)
{
  gl_Position = vec4(in_vertex.x, in_vertex.z, 0.0, 1.0);
}
