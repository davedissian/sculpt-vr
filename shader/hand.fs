#version 330 core

out vec4 colour;
uniform vec4 u_colour;

void main(void)
{
	colour = u_colour;
}
