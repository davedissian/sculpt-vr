#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstring>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <set>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <exception>
#include <fstream>

#ifdef __APPLE__
# include <OpenGL/gl3.h>
#else
# include <GL/glew.h>
#endif
#include <SDL2/SDL.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shader.h"


#endif /* __COMMON_H__ */
