#ifndef __GRAPHICS_TYPES_HPP__
#define __GRAPHICS_TYPES_HPP__

#include <stdint.h>

class Shader
{
public:
  Shader(const char* vertex_path, const char* fragment_path);
  ~Shader();

public:
  inline const uint32_t Id() const { return _id; }
  
  void Bind();
  static void Unbind();

private:
  uint32_t _id;
};

class Texture
{
public:
  Texture(const char* path, bool flip = false);
  ~Texture();

public:
  inline const uint32_t Id() const { return _id; }
  inline const int GetWidth() const { return _width; }
  inline const int GetHeight() const { return _height; }
  inline const char* GetPath() const { return _path; }

  void Bind(const int offset);
  static void Unbind();

private:
  uint32_t _id;
  int _width;
  int _height;
  char* _path;
};

#endif // __GRAPHICS_TYPES_HPP__