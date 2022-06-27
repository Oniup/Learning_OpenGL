#ifndef __GRAPHICS_TYPES_HPP__
#define __GRAPHICS_TYPES_HPP__

#include <stdint.h>

class Shader
{
public:
  Shader(const char* vertex_path, const char* fragment_path);
  ~Shader();

public:
  inline const uint32_t id() const { return _id; }
  
  void bind();
  void unbind();

private:
  uint32_t _id;
};

class Texture
{
public:
  Texture(const char* png_path, bool flip = false);
  ~Texture();

public:
  inline const uint32_t id() const { return _id; }
  inline const int width() const { return _width; }
  inline const int height() const { return _height; }

  void bind(const int offset);
  void unbind();

private:
  uint32_t _id;
  int _width;
  int _height;
};

#endif // __GRAPHICS_TYPES_HPP__