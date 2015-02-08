#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_colour;

uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;

out vec3 v_normal;
out vec4 v_colour;

void main(void)
{
  v_normal = in_normal;
  v_colour = in_colour;
  gl_Position = u_proj * u_view * u_model * vec4(in_vertex, 1.0f);
}
