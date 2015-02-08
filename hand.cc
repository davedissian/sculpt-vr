// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"

static const Vertex PLANE_MESH[] =
{
  { -0.3f, 0.1f, -0.3f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF}, 
  {  0.3f, 0.0f, -0.3f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  {  0.3f, 0.1f,  0.3f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},

  { -0.3f, 0.0f,  0.3f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  { -0.3f, 0.1f, -0.3f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  {  0.3f, 0.0f,  0.3f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF}
};

Hand::Hand(const Type& type)
  : model(1.0f)
  , type(type)
{
}


Hand::~Hand()
{
}


void Hand::create()
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


void Hand::render(Shader& shader)
{
  glm::vec4 colour;

  switch (type) {
    case Type::LEFT: {
      colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      break;
    }
    case Type::RIGHT: {
      colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
      break;
    }
  }

  std::cout << wrist.x << " " << wrist.y << " " << wrist.z << std::endl;
  glBindVertexArray(vao);
  shader.uniform("u_model", glm::translate(wrist));
  glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Hand::destroy()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}


bool Hand::update(const Leap::Hand& hand)
{
  glm::vec3 newWrist(hand.wristPosition().x, -hand.wristPosition().y, hand.wristPosition().z);
  wrist = newWrist / 100.0f;
  model = glm::translate(wrist);
  tracked = true;
  return true;
}
