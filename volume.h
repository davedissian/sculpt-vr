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
} __attribute__((packed));

class Volume
{
public:
  Volume(size_t size) 
    : size(size)
    , X_OFFSET(size * size)
    , Y_OFFSET(size)
  {
    grid.resize(size * size * size);
    memset(&grid[0], 0, sizeof(Point) * size * size * size);

    // Fill with dummy data
    for (size_t i = 0; i < (size * size * size); ++i)
    {
      grid[i].isoValue = 0;
    }
  }

  /**
   * Converts the grid into a list of triangles.
   *
   * @param size Grid dimension
   * @param out List where the triangles should be inserted
   */
  void GridToTris(std::vector<Triangle>& out);

  /**
   * Fills in a cube with edge of length edge_len and the bottom left corner 
   * at (x, y, z) with the given isoValue and rbga data.
   *
   * @param x X coordinate of the bottom left corner
   * @param y y coordinate of the bottom left corner
   * @param z z coordinate of the bottom left corner
   * @param edge_len Edge length
   * @param isoValue Iso value
   * @return true if the cube changed, false otherwise.              
   */
  bool FillCube(size_t x, size_t y, size_t z, size_t edge_len, float isoValue);

  /**
   * Fills in a cube centered at (x, y, z) with 
   * the given isoValue and rbga data.
   *
   * @param x X coordinate of the bottom left corner
   * @param y y coordinate of the bottom left corner
   * @param z z coordinate of the bottom left corner
   * @param radius Radius
   * @param isoValue Iso value
   * @return true if the sphere changed, false otherwise.     
   */
  bool FillSphere(size_t x, size_t y, size_t z, size_t radius, float isoValue);

private:
  void VoxelToTris(size_t x, size_t y, size_t z, std::vector<Triangle>& out);

  /* 3D point grid. */
  std::vector<Point> grid;
  
  /* grid size. */
  size_t size;

  /* Coordinate offsets. */
  const uint32_t X_OFFSET;
  const uint32_t Y_OFFSET;

};


#endif
