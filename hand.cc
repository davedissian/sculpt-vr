// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"

static glm::mat4 mapLeapToWorld(
    -1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

static const Vertex CUBE_MESH[] =
{
  {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
  {0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
  {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
  {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
  {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
  {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
  {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
  {0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
  {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
  {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
  {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
  {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
  {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
  {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
  {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
  {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
  {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
  {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
  {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
  {0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
  {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
  {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
  {0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
  {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
  {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
  {0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
  {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
  {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
  {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
  {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
  {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
  {0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
  {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
  {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
  {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
  {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f}
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

glm::mat4 placeCubeBetween(glm::vec3 a, glm::vec3 b, float thickness)
{
  glm::vec3 delta = b - a;
  float length = glm::length(delta);
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
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
}


bool Hand::render(Shader& shader, const glm::mat4& headMatrix)
{
  glm::vec4 colour;
  bool update = false;

  glm::mat4 xform = headMatrix * mapLeapToWorld;

  // Points [i][j][k] (i is the finger index, j is the bone index (4 is the tip), k is the front/back
  glm::vec3 indexTip(xform * glm::vec4(points[1][4][0], 1.0f));
  glm::vec3 p = (indexTip + volume.HalfSize() - volume.GetPosition()) * 64.0f / 3.0f;

  switch (type) {
    case Type::LEFT: {
      colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
      if (volume.FillCube(p.x, p.y, p.z, 2, 1) && 
          volume.HasNeighbours(p.x, p.y, p.z)) {
        update = true;
      }
      break;
    }
    case Type::RIGHT: {
      colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
      if (volume.FillCube(p.x, p.y, p.z, 3, 0) && 
          volume.HasNeighbours(p.x, p.y, p.z)) {
        update = true;
      }
      break;
    }
    default:
      break;
  }

  glBindVertexArray(vao);
  
  // Draw cuboids for each bone
  for (int i = 0; i < 5; i++)
  {
    // Draw joints
    shader.uniform("u_colour", colour);
    for (int j = 0; j < 4; j++)
    {
      glm::vec3 a(xform * glm::vec4(points[i][j][0], 1.0f));
      glm::vec3 b(xform * glm::vec4(points[i][j][1], 1.0f));
      shader.uniform("u_model", placeCubeBetween(a, b, 0.05f));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
  }
  
  shader.uniform("u_colour", glm::vec4(0.75f, 0.0f, 0.0f, 1.0f));
  shader.uniform("u_model", glm::translate(indexTip) * glm::scale(glm::vec3(0.05f)));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  return update;
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
      points[i][j][0] = glm::vec3(p.x, p.y, p.z) / 200.0f;
      points[i][j][1] = glm::vec3(p2.x, p2.y, p2.z) / 200.0f;
    }

    auto tip = (*f).tipPosition();
    points[i][4][0] = glm::vec3(tip.x, tip.y, tip.z) / 200.0f;
  }
  wrist = glm::vec3(hand.wristPosition().x, hand.wristPosition().y, hand.wristPosition().z) / 200.0f;
  tracked = true;
  return true;
}
