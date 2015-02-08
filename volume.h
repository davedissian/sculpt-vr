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
    : realSize(0.4f)
    , size(size)
    , X_OFFSET(size * size)
    , Y_OFFSET(size)
  {
    grid.resize(size * size * size);
    memset(&grid[0], 0, sizeof(Point) * size * size * size);
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

  /**
   * Exports the model to an .obj file.
   *
   * @param file Name of the file to write to.
   */
  static void ExportOBJ(
      const std::string& file, 
      const std::vector<Triangle>& trgs);

  /**
   * Clears the volume i.e. sets iso values to 0.
   */
  void ClearVolume(void);

  /**
   * Determines if any of the neighbouring points are filled.
   *
   * @param x X coordinate
   * @param y y coordinate
   * @param z z coordinate   
   * @return true if any of the neighbouring points are filled, false otherwise
   */
  bool HasNeighbours(size_t x, size_t y, size_t z);

  void SetPosition(const glm::vec3& p) { position = p; }
  glm::vec3 GetPosition() const { return position; }
  glm::vec3 HalfSize() const { return glm::vec3(realSize * 0.5f); }
  float RealSize() const { return realSize; }
  size_t Detail() const { return size; }

private:
  void VoxelToTris(size_t x, size_t y, size_t z, std::vector<Triangle>& out);

  /* 3D point grid. */
  std::vector<Point> grid;
  
  /* grid size. */
  float realSize;
  size_t size;

  /* Coordinate offsets. */
  const uint32_t X_OFFSET;
  const uint32_t Y_OFFSET;

  glm::vec3 position;

  void Interpolate(float x1, float y1, float z1, Point& p1, 
    float x2, float y2, float z2, Point& p2, 
    Vertex& out);

};


#endif
