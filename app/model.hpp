#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "app/mesh.hpp"
#include "app/transform.hpp"

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
  Transform transform;

public:
  Model();
  Model(const char* path);
  Model(const char* path, Shader* shader);
  ~Model();

public:
  void SetShader(Shader* shader);
  void Render(std::vector<Light>& lights, const glm::mat4& view, const glm::mat4& projection);

private:
  Shader* _shader;

  std::vector<Mesh> _meshes;
  const char* _directory;

private:
  void _LoadModel(const char* path);
  void _ProcessNode(aiNode* node, const aiScene* scene);
  Mesh _ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture*> _LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::vector<Texture*>* loaded_textures);

};

#endif // __MODEL_HPP__