// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __SPHERE_H__
#define __SPHERE_H__

class Sphere
{
public:
  Sphere(float radius);

  void create();
  void render(Shader& shader);
  void destroy();

private:
  glm::mat4 model;
  GLuint vao;
  GLuint vbo;
};

#endif /*__SPHERE_H__*/