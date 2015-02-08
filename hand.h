// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __HAND_H__
#define __HAND_H__


class Hand
{
public:
	enum class Type {
		LEFT,
		RIGHT,
		CHERNOBYL
	};
public:
	Hand(const Type& type);
	~Hand();

  void create();
  void render(Shader& shader, glm::mat4 headMatrix);
  void destroy();
  bool update(const Leap::Hand& hand);

	glm::mat4 model;
  GLuint vao;
  GLuint vbo;

public:
	bool tracked = false;
	glm::vec3 wrist;
	Type type;
};


#endif /*__HAND_H__*/
