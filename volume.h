// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __VOLUME_H__
#define __VOLUME_H__

#include "common.h"
#include "mesh.h"

struct Point 
{
  float isoValue;
  uint8_t r, g, b, a;
} __attribute__((packed));


class Volume
{
public:

  static_assert(sizeof(Point) == 8, "Invalid point size.");

  Volume(size_t size) {
    grid.resize(size * size * size);
    memset(&grid[0], 0, sizeof(Point) * size * size * size);

    // Fill with dummy data
    for (size_t i = 0; i < (size * size * size); ++i)
    {
      grid[i].isoValue = 1;
      grid[i].r = 0xff;
      grid[i].g = 0xff;
      grid[i].b = 0xff;
      grid[i].a = 0xff;
    }
  }

  /**
   * Converts the grid into a list of triangles.
   *
   * @param size Grid dimension
   * @param out List where the triangles should be inserted
   */
  void GridToTris(size_t size, std::vector<Triangle>& out);

private:
  void VoxelToTris(size_t x, size_t y, size_t z, std::vector<Triangle>& out);

  // 3D point grid.
  std::vector<Point> grid;
};


#endif
