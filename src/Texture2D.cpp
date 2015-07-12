#include "Texture2D.h"

#include <iostream>

#include "targa.h"

Texture2D::Texture2D()
{
}

Texture2D::Texture2D(const char* _pImgFilename)
{
  m_TextureFilename = _pImgFilename;
}

void Texture2D::init(const char* _pImgFilename)
{
  TargaImage textureImg;

  m_TextureFilename = _pImgFilename;
  if (!textureImg.load(_pImgFilename))
  {
    std::cerr << "Could not load the texture" << std::endl;
    b_Initialized = false;
    return;
  }

  glGenTextures(1, &m_ID);
  glBindTexture(GL_TEXTURE_2D, m_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureImg.getWidth(),
    textureImg.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg.getImageData());

  b_Initialized = true;
}

void Texture2D::init()
{
  TargaImage textureImg;

  if (!textureImg.load(m_TextureFilename))
  {
    std::cerr << "Could not load the texture" << std::endl;
    b_Initialized = false;
    return;
  }

  glGenTextures(1, &m_ID);
  glBindTexture(GL_TEXTURE_2D, m_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureImg.getWidth(),
    textureImg.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg.getImageData());
  glBindTexture(GL_TEXTURE_2D, 0);

  b_Initialized = true;
}

void Texture2D::use()
{
  //if (b_Initialized)
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::deinit()
{
  glDeleteTextures(1, &m_ID);
}

Texture2D::~Texture2D()
{
}