// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __MESH_H__
#define __MESH_H__


struct Vertex {
  float x, y, z;
  float nx, ny, nz;
};

struct Triangle {
  Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
    : v0(v0)
    , v1(v1)
    , v2(v2)
  {
  }

  Triangle()
  {
  }

  Vertex v0, v1, v2;
};


#endif /*__MESH_H__*/