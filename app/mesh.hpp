#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <glm/glm.hpp>
#include <stdint.h>
#include <string>
#include <vector>

class Shader;
class Texture;
class Light;
class Transform;

struct MeshVertex
{
  glm::vec3 position;
  glm::vec2 uv;
  glm::vec3 normal;
};

enum MeshTextureType
{
  MESH_TEXTURE_TYPE_DIFFUSE = 0,
  MESH_TEXTURE_TYPE_SPECULAR,
};

struct Material
{
  std::vector<Texture> diffuse;
  std::vector<Texture> specular;
  float shininess;
};

class Mesh
{
public:
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  Material material;

public:
  Mesh();
  Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const Material& material);
  ~Mesh();

public:
  void Render(Shader* shader, const glm::mat4& view, const glm::mat4& projection, const std::vector<Light>& lights, const Transform* transform);

private:
  uint32_t _vao, _vbo, _ebo;

private:
  void _InitVertexData();
  void _Clear();
};

#endif // __MESH_HPP__