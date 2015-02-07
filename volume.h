// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#ifndef __VOLUME_H__
#define __VOLUME_H__

template<size_t N>
class Volume
{
public:
  struct Point 
  {
    float isoValue;
    uint8_t r, g, b, a;
  } [[gnu::packed]];

  static_assert(sizeof(Point) == 8, "Invalid voxel size.");
  static_assert((N & (N - 1)) == 0, "Invalid volume size.");

  Volume() {
    voxels.resize(N * N * N);
    memset(&voxels[0], 0, sizeof(Point) * N * N * N);
  }

  void ToMesh(std::vector<Triangle>& out);

private:
  void VoxelToTris(size_t x, size_t y, size_t z, std::vector<Triangle>& out);

  /// 3D point grid.
  std::vector<Point> grid;
};


#endif
