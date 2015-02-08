// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"

static const Vertex CUBE_MESH[] =
{
  {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0xFF, 0xFF},
  {0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0xFF, 0xFF},
  {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0xFF, 0xFF},
  {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0xFF, 0xFF},
  {0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0xFF, 0xFF},
  {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0xFF, 0xFF},
  {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0xFF, 0xFF},
  {0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0xFF, 0xFF},
  {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0xFF, 0xFF},
  {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0xFF, 0xFF},
  {0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0xFF, 0xFF},
  {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0xFF, 0xFF},
  {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0xFF, 0xFF},
  {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0xFF, 0xFF}
};

Hand::Hand(const Type& type)
  : model(1.0f)
  , type(type)
{
}


Hand::~Hand()
{
}

glm::mat4 placeCubeBetween(glm::vec3 a, glm::vec3 b, float thickness)
{
  glm::vec3 delta = b - a;
  float length = glm::length(delta);
  float angle = std::acos(delta.y / length);
  return glm::translate((a + b) * 0.5f)
    * glm::mat4_cast(glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), delta / length))
    * glm::scale(glm::vec3(thickness, length, thickness));
}

void Hand::create()
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_MESH), CUBE_MESH, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*)12);
  glVertexAttribPointer(2, 4, GL_UNSIGNED_SHORT, GL_FALSE, 32, (void*)24);
}


void Hand::render(Shader& shader, glm::mat4 headMatrix)
{
  glm::vec4 colour;

  switch (type) {
    case Type::LEFT: {
      colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
      break;
    }
    case Type::RIGHT: {
      colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
      break;
    }
    default:
      break;
  }

  glBindVertexArray(vao);
  shader.uniform("u_colour", colour);
  
  // Draw cuboids for each bone
  for (int i = 0; i < 5; i++)
  {
    // Exclude the last point
    for (int j = 0; j < 4; j++)
    {
      glm::vec3 a(headMatrix * glm::vec4(points[i][j][0], 1.0f)); a.y *= -1.0f;
      glm::vec3 b(headMatrix * glm::vec4(points[i][j][1], 1.0f)); b.y *= -1.0f;
      shader.uniform("u_model", placeCubeBetween(a, b, 0.1f));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
  }

  glm::vec4 absHand = headMatrix * glm::vec4(wrist, 1.0f);
  shader.uniform("u_model", glm::translate(glm::vec3(absHand.x, -absHand.y, absHand.z)) * glm::scale(glm::vec3(0.1f)));
  glDrawArrays(GL_TRIANGLES, 0, 36);
}


void Hand::destroy()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}


bool Hand::update(const Leap::Hand& hand)
{
  // For each finger
  Leap::FingerList fingers = hand.fingers();
  for (auto f = fingers.begin(); f != fingers.end(); f++)
  {
    int i = (int)(*f).type();
    
    // For each bone
    for (int j = 0; j < 4; j++)
    {
      auto bone = (*f).bone((Leap::Bone::Type)j);
      auto p = bone.prevJoint();
      auto p2 = bone.nextJoint();
      points[i][j][0] = glm::vec3(-p.x, p.z, -p.y) / 100.0f;
      points[i][j][1] = glm::vec3(-p2.x, p2.z, -p2.y) / 100.0f;
    }
  }
  wrist.x = -hand.wristPosition().x / 100.0f;
  wrist.y = hand.wristPosition().z / 100.0f;
  wrist.z = -hand.wristPosition().y / 100.0f;
  tracked = true;
  return true;
}
