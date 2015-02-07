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
	enum class Type {
		VERT,
		FRAG,
		GEOM
	};
public:
  Shader(const std::string& name);
  virtual ~Shader();

  void compile(const std::string& filel, const Type& type);
  void link();

  template<typename T>
  void uniform(const std::string& name, const T& t);

private:
  std::unordered_map<std::string, GLuint> uniforms;

};

template<>
inline void Shader::uniform<glm::mat4>(const std::string& name, const glm::mat4& t)
{

}

#endif /*__SHADER_H__*/