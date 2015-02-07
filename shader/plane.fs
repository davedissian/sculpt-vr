#version 330 core

out vec4 colour;
in vec3 v_vertex;

void main(void)
{
	vec3 dist = abs(v_vertex - floor(v_vertex));

	if (dist.x < 0.05f || dist.z < 0.05f) {
  	colour = vec4(0.046, 0.53, 0.66, 1);
	} else {
		colour = vec4(0.01, 0.09, 0.2, 1);
	}
}
