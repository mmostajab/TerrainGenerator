#ifndef __TEXTURE2D__
#define __TEXTURE2D__

#include <GL/glew.h>
#include <string>

class Texture2D
{
private:
  GLuint m_ID;
  std::string m_TextureFilename;

  bool b_Initialized;

  int m_PreResW, m_PreResH;
  int m_RenderTexW, m_RenderTexH;

public:
  Texture2D();
  Texture2D(const char* _pImgFilename);

  void init(const char* _pImgFilename);
  void init();

  void use();

  void deinit();

  ~Texture2D();
};

#endif