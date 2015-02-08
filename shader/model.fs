#version 330 core

const vec3 LIGHT_DIR = vec3(-0.74f, 0.0f, -0.74f);

out vec4 colour;

in vec3 v_normal;
in vec4 v_colour;

void main(void)
{
	colour = vec4(max(dot(normalize(v_normal), LIGHT_DIR), 0.05));
}
