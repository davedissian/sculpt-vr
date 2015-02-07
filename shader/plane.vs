#version 330

in vec3 in_vertex;

uniform mat4 u_view;
uniform mat4 u_proj;

void main(void)
{
  gl_Position = u_proj * u_view * vec4(in_vertex, 1.0f); 
}
