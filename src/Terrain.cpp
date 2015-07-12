#include "Terrain.h"

#include "helper.h"

GLuint Terrain::shader_program = 0;

Terrain::Terrain() {

  width = 0.1f;
  depth = 0.4f;

  tiles_x = 10;
  tiles_y = 1;
  tiles_z = 1;
}

Terrain::~Terrain() {

}

void Terrain::compileShader() {
  shader_program = compile_link_vs_fs("../../src/glsl/terrain.vert", "../../src/glsl/terrain.frag");
}

void Terrain::setPoints(const std::vector<glm::vec4>& _points) {


  above_texture.init("../../media/above.tga");
  side_texture.init("../../media/side.tga");

  int npoints = static_cast<int>(_points.size());

  points.resize(4 * _points.size());
  normals.resize(4 * _points.size());
  texcoords.resize(4 * _points.size());
  for (size_t i = 0; i < 4; i++){
    for (size_t j = 0; j < _points.size(); j++){
      points[i * _points.size() + j] = glm::vec3(
        _points[j].x, 
        _points[j].y + (i % 2 == 0 ? 0.0f : width), 
        _points[j].z + (i < 2 ? 0.0f : depth));

      texcoords[i * _points.size() + j] = glm::vec3(
        static_cast<float>(j) / (npoints - 1) * tiles_x,
        i % 2 == 0 ? 0.0f : 1.0f * tiles_y,
        i < 2 ? 0.0f : 1.0f * tiles_z
        );
    }
  }

  indices.clear();
  glm::vec3 n;
  for (int j = 0; j < npoints - 1; j++){
    
    // Cap in the beginning of mesh
    if (j == 0){
      indices.push_back(j);
      indices.push_back(j + 1 * npoints);
      indices.push_back(j + 2 * npoints);
      n = glm::normalize(glm::cross(points[j + 1 * npoints] - points[j], points[j + 2 * npoints] - points[j]));
      
      indices.push_back(j + 2 * npoints);
      indices.push_back(j + 1 * npoints);
      indices.push_back(j + 3 * npoints);

      normals[j] += n;
      normals[j + 1 * npoints] += n;
      normals[j + 2 * npoints] += n;
      normals[j + 3 * npoints] += n;
    }

    // Cap end of mesh
    if (j == _points.size() - 2){
      indices.push_back(j + 1);
      indices.push_back(j + 1 + 1 * npoints);
      indices.push_back(j + 1 + 2 * npoints);
      n = glm::normalize(glm::cross(points[j + 1 + 2 * npoints] - points[j + 1], points[j + 1 + 1 * npoints] - points[j + 1]));

      indices.push_back(j + 1 + 2 * npoints);
      indices.push_back(j + 1 + 1 * npoints);
      indices.push_back(j + 1 + 3 * npoints);

      normals[j + 1] += n;
      normals[j + 1 + 1 * npoints] += n;
      normals[j + 1 + 2 * npoints] += n;
      normals[j + 1 + 3 * npoints] += n;
    }

    // Right Side
    indices.push_back(j);
    indices.push_back(j + 1 * npoints);
    indices.push_back(j + 1);
    n = glm::normalize(glm::cross(points[j + 1] - points[j], points[j + 1 * npoints] - points[j]));    

    indices.push_back(j + 1);
    indices.push_back(j + 1 * npoints);
    indices.push_back(j + 1 + 1 * npoints);

    normals[j] += n;
    normals[j + 1 * npoints] += n;
    normals[j + 1] += n;
    normals[j + 1 + 1 * npoints] += n;

    // Left Side
    indices.push_back(j + 2 * npoints);
    indices.push_back(j + 3 * npoints);
    indices.push_back(j + 1 + 2 * npoints);
    n = glm::normalize(glm::cross(points[j + 3 * npoints] - points[j + 2 * npoints], points[j + 1 + 2 * npoints] - points[j + 2 * npoints]));

    indices.push_back(j + 1 + 2 * npoints);
    indices.push_back(j + 3 * npoints);
    indices.push_back(j + 1 + 3 * npoints);

    normals[j + 2 * npoints] += n;
    normals[j + 1 + 2 * npoints] += n;
    normals[j + 3 * npoints] += n;
    normals[j + 1 + 3 * npoints] += n;

    // Bottom
    indices.push_back(j);
    indices.push_back(j + 2 * npoints);
    indices.push_back(j + 1);
    n = glm::normalize(glm::cross(points[j + 2 * npoints] - points[j], points[j + 1] - points[j]));

    indices.push_back(j + 1);
    indices.push_back(j + 2 * npoints);
    indices.push_back(j + 1 + 2 * npoints);

    normals[j] += n;
    normals[j + 2 * npoints] += n;
    normals[j + 1] += n;
    normals[j + 1 + 2 * npoints] += n;

    // Top
    indices.push_back(j + 1 * npoints);
    indices.push_back(j + 3 * npoints);
    indices.push_back(j + 1 + 1 * npoints);
    n = glm::normalize(glm::cross(points[j + 1 + 1 * npoints] - points[j + 1 * npoints], points[j + 3 * npoints] - points[j + 1 * npoints]));

    indices.push_back(j + 1 + 1 * npoints);
    indices.push_back(j + 3 * npoints);
    indices.push_back(j + 1 + 3 * npoints);
    
    normals[j + 1 * npoints] += n;
    normals[j + 1 + 1 * npoints] += n;
    normals[j + 3 * npoints] += n;
    normals[j + 1 + 3 * npoints] += n;

  }

  for (size_t i = 0; i < normals.size(); i++)
    normals[i] = glm::normalize(normals[i]);

  // Create GL points buffer
  glGenBuffers(1, &points_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, points_buffer);
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &normals_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &texcoord_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec3), texcoords.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &indices_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

}

void Terrain::draw() {

  glUseProgram(shader_program);
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, points_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, BUFFER_OFFSET(0));
  glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NULL, BUFFER_OFFSET(0));
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, NULL, BUFFER_OFFSET(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);

  // Bind textures.
  glActiveTexture(GL_TEXTURE0);
  side_texture.use();
  
  glActiveTexture(GL_TEXTURE1);
  above_texture.use();

  //glDrawArrays( GL_LINE_STRIP, 0, static_cast<GLsizei>( points.size() ) );
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

}