// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __PLANE_H__
#define __PLANE_H__


class Plane
{
public:
  Plane(float width, float depth, size_t splitX, size_t splitY);

  void create();
  void render(const Shader& shader);
  void destroy();

private:
	glm::mat4 model;
  GLuint vao;
  GLuint vbo;
};


#endif /*__PLANE_H__*/