// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"

Shader::Shader(const std::string& name)
  : name(name)
{

}


Shader::~Shader()
{

}


void Shader::compile(const std::string& file, const Type& type)
{
  GLenum shaderType;
  GLuint shader;
  GLint status;

  switch (type) {
    case Type::VERT: shaderType = GL_VERTEX_SHADER;   break;
    case Type::FRAG: shaderType = GL_FRAGMENT_SHADER; break;
    case Type::GEOM: shaderType = GL_GEOMETRY_SHADER; break;
  }

  if (!(shader = glCreateShader(shaderType))) {
    throw std::runtime_error("Cannot create shader.");
  }
  shaders.push_back(shader);

  std::ifstream source(file);
  std::string str(
      (std::istreambuf_iterator<char>(source)),
      std::istreambuf_iterator<char>());

  const char *ptr = &str[0];
  glShaderSource(shader, 1, &ptr, nullptr);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
    std::string info;
    info.resize(status);
    glGetShaderInfoLog(shader, status, nullptr, &info[0]);
    throw std::runtime_error(info);
  }
}


void Shader::link()
{
  GLint status;
  GLint maxl;

  if (!(prog = glCreateProgram())) {
    throw std::runtime_error("Cannot create program.");
  }

  for (const GLuint& shader : shaders) {
    glAttachShader(prog, shader);
  }
  glLinkProgram(prog);

  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (!status) {
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &status);
    std::string info;
    info.resize(status);
    glGetProgramInfoLog(prog, status, nullptr, &info[0]);
    throw std::runtime_error(info);
  }

  glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &status);
  glGetProgramiv(prog, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxl);
  for (auto i = 0; i < status; ++i) {
    std::string name;
    GLsizei len;
    GLint size;
    GLenum type;

    name.resize(maxl);
    glGetActiveUniform(prog, i, maxl, &len, &size, &type, &name[0]);
    while (*name.rbegin() == '\0') {
      name.pop_back();
    }
    uniforms[name] = glGetUniformLocation(prog, name.c_str());
  }
}


void Shader::destroy()
{
  for (const GLuint& shader : shaders) {
    glDetachShader(prog, shader);
    glDeleteShader(shader);
  }
  glDeleteProgram(prog);
}
