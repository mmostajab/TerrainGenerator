#include "Quad.h"

#include "helper.h"

GLuint Quad::shader_program = 0;

Quad::Quad() {

  width   = 1.0f;
  height  = 1.0f;

  tiles_x = 3;
  tiles_y = 3;
}

Quad::~Quad() {

}

void Quad::compileShader() {
  shader_program = compile_link_vs_fs("../../src/glsl/quad.vert", "../../src/glsl/texture_quad.frag");
}

void Quad::init(const float& w, const float& h, const std::string& texture_filename) {


  texture.init(texture_filename.c_str());

  // Create quad buffer
  glm::vec4 quad_points[4] = {
    glm::vec4(-w/2,  h/2, -1, 1),
    glm::vec4(-w/2, -h/2, -1, 1),
    glm::vec4( w/2,  h/2, -1, 1),
    glm::vec4( w/2, -h/2, -1, 1),
  };
  glm::vec2 quad_coords[4] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, tiles_y),
    glm::vec2(tiles_x, 0.0f),
    glm::vec2(tiles_x, tiles_y)
  };
  glGenBuffers(1, &points_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, points_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), quad_points, GL_STATIC_DRAW);

  glGenBuffers(1, &texcoord_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), quad_coords, GL_STATIC_DRAW);

}

void Quad::draw() {

  glUseProgram(shader_program);
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, points_buffer);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, BUFFER_OFFSET(0));
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, NULL, BUFFER_OFFSET(0));

  // Bind textures.
  glActiveTexture(GL_TEXTURE0);
  texture.use();

  //glDrawArrays( GL_LINE_STRIP, 0, static_cast<GLsizei>( points.size() ) );
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}