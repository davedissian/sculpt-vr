// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"

static const Vertex PLANE_MESH[] =
{
  { -0.3f, 0.1f, -0.3f, 0.0f, 1.0f, 0.0f}, 
  {  0.3f, 0.0f, -0.3f, 0.0f, 1.0f, 0.0f},
  {  0.3f, 0.1f,  0.3f, 0.0f, 1.0f, 0.0f},

  { -0.3f, 0.0f,  0.3f, 0.0f, 1.0f, 0.0f},
  { -0.3f, 0.1f, -0.3f, 0.0f, 1.0f, 0.0f},
  {  0.3f, 0.0f,  0.3f, 0.0f, 1.0f, 0.0f}
};

Hand::Hand(const Type& type, Volume& volume)
  : model(1.0f)
  , type(type)
  , volume(volume)
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
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
}


bool Hand::render(Shader& shader, const glm::mat4& headMatrix)
{
  glm::vec4 colour;
  bool update = false;

  glm::vec3 p = (wrist + glm::vec3(0, 0, 0)) * 64.0f / 3.0f;

  switch (type) {
    case Type::LEFT: {
      colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
      std::cerr << p.x << " " << p.y << " " << p.z << std::endl;
      if (volume.FillCube(p.x, p.y, p.z, 5, 1, 0xff, 0xff, 0xff, 0xff)) {
        update = true;
      }
      break;
    }
    case Type::RIGHT: {
      colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
      break;
    }
  }

  glm::vec4 absHand = headMatrix * glm::vec4(wrist, 1.0f);

  glBindVertexArray(vao);
  shader.uniform("u_model", glm::translate(glm::vec3(absHand.x, -absHand.y, absHand.z)));
  shader.uniform("u_colour", colour);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  return update;
}


void Hand::destroy()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}


bool Hand::update(const Leap::Hand& hand)
{
  glm::vec3 newWrist(-hand.wristPosition().x, hand.wristPosition().z, -hand.wristPosition().y);
  wrist = newWrist / 100.0f;
  tracked = true;
  return true;
}
