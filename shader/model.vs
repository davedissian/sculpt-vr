#version 330 core

layout(location = 0) in vec3 in_vertex;

uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;

void main(void)
{
  gl_Position = u_proj * u_view * u_model * vec4(in_vertex, 1.0f);
}
