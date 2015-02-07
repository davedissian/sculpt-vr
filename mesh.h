// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __MESH_H__
#define __MESH_H__


struct Vertex {
  float x, y, z;
  float nx, ny, nz;
  uint16_t r, g, b, a;
};

struct Triangle {
  Vertex v[3];
};


#endif /*__MESH_H__*/