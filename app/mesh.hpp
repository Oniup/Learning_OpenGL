#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <glm/glm.hpp>

#include <stdint.h>
#include <string>
#include <vector>

class Texture;
class Shader;
struct Transform;
struct Light;

struct Vertex
{
  glm::vec3 vertex;
  glm::vec2 uv;
  glm::vec3 normal;
};

struct Material
{
  std::vector<Texture*> diffuse;
  std::vector<Texture*> specular;
  int shininess;
};

class Mesh
{
public:
  Mesh();
  Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> indices, const Material& material, bool init_vertex_data = true);
  ~Mesh();

  Mesh& operator=(const Mesh& mesh);

public:
  inline const Material* GetMaterial() const { return &_material; }
  inline Material* GetMaterial() { return &_material; }
  inline const std::vector<Vertex>* GetVertices() const { return &_vertices; }
  inline const std::vector<uint32_t>* GetIndices() const { return &_indices; }

  static Mesh* GenerateCube(const Material& material);

public:
  void Render(Shader* shader, const Transform& transform, const std::vector<Light>& lights, const glm::mat4& view, const glm::mat4& projection);
  void Free();

private:
  Material _material;
  std::vector<Vertex> _vertices;
  std::vector<uint32_t> _indices;
  uint32_t _vao, _vbo, _ebo;

private:
  void _InitVertexData();

};

#endif // __MESH_HPP__