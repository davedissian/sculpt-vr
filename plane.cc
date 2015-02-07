// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"

static const Vertex PLANE_MESH[] =
{
  { -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF}, 
  {  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  {  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},

  { -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  { -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  {  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF}
};


Plane::Plane(float width, float depth)
  : model(glm::scale(glm::vec3(width, 0.0f, depth)))
{
}

void Plane::create()
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_MESH), PLANE_MESH, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*)12);
  glVertexAttribPointer(2, 4, GL_UNSIGNED_SHORT, GL_FALSE, 32, (void*)24);
}


void Plane::render(Shader& shader)
{
  glBindVertexArray(vao);
  shader.uniform("u_model", model);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Plane::destroy()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}


