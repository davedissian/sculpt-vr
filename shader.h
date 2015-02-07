// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <unordered_map>


class Shader
{
public:


private:
  std::unordered_map<std::string, GLuint> uniforms;

};


#endif /*__SHADER_H__*/