

uniform mat4 u_view;
uniform mat4 u_proj;

void main(void)
{
  gl_Position = u_proj * u_view * vec4(0, 0, 0, 1); 
}