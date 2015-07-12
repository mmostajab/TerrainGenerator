#ifndef __TERRAIN_H__
#define __TERRAIN_H__

// STD
#include <vector>

// GL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2D.h"

class Terrain {
public:
  
  Terrain();
  ~Terrain();

  static void compileShader();

  void setPoints(const std::vector<glm::vec4>& _points);

  void draw();
  void draw_without_assigning_shader();
  

private:

  float width, depth;

  // Points of the Terrain
  std::vector<glm::vec3> points;
  std::vector<glm::vec3> texcoords;
  std::vector<glm::vec3> normals;
  std::vector<uint32_t>  indices;

  // GL Buffers
  static GLuint shader_program;
  GLuint points_buffer, normals_buffer, indices_buffer, texcoord_buffer;

  // Textures
  float tiles_x, tiles_y, tiles_z;
  Texture2D above_texture;
  Texture2D side_texture;

};

#endif // __TERRAIN_H__