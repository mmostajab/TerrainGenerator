#ifndef __QUAD_H__
#define __QUAD_H__

// STD
#include <vector>
#include <string>

// GL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2D.h"

class Quad {
public:
  
  Quad();
  ~Quad();

  static void compileShader();

  void init(const float& w, const float& h, const std::string& texture_filename);

  void draw();
  

private:

  float width, height;

  // Points of the Terrain
  glm::vec4 points[4];
  glm::vec2 texcoords[4];
  
  // GL Buffers
  static GLuint shader_program;
  GLuint points_buffer, texcoord_buffer;

  // Textures
  float tiles_x, tiles_y;
  Texture2D texture;

};

#endif // __QUAD_H__