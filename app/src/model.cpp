#include "app/model.hpp"

#include "app/graphics_types.hpp"
#include "app/utils.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model()
  : transform({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f}), 
    _shader(nullptr), _meshes(), _directory(nullptr)
{
}

Model::Model(const char* path)
  : Model(path, nullptr)
{
}

Model::Model(const char* path, Shader* shader)
{
  _shader = shader;

  _LoadModel(path);
}

Model::~Model()
{
  for (Mesh& mesh : _meshes)
  {
    for (int i = 0; i < 2; i++)
    {
      std::vector<Texture*>* textures;
      Material* material = mesh.GetMaterial();
      switch (i)
      {
        case 0: textures = &material->diffuse; break;
        case 1: textures = &material->specular; break;
      }

      for (int i = 0; i < textures->size(); i++)
        delete textures->at(i);
    }
  }

  delete[] _directory;
}

void Model::SetShader(Shader* shader)
{
  _shader = shader;
}

void Model::Render(std::vector<Light>& lights, const glm::mat4& view, const glm::mat4& projection)
{
  for (Mesh& mesh : _meshes)
    mesh.Render(_shader, transform, lights, view, projection);
}

void Model::_LoadModel(const char* path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
  
  if (!scene)
  {
    printf("Failed to Load Model: %s\n", importer.GetErrorString());
    exit(-1);
  }

  _directory = utils::GetPathDirectory(path);

  _ProcessNode(scene->mRootNode, scene);
}

void Model::_ProcessNode(aiNode* node, const aiScene* scene)
{
  for (int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    _meshes.emplace_back(std::move(_ProcessMesh(mesh, scene)));
  }

  for (int i = 0; i < node->mNumChildren; i++)
  {
    _ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::_ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  Material material;

  // loading vertices
  for (int i = 0; i < mesh->mNumVertices; i++)
  {
    vertices.push_back({
      glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
      (mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f)),
      glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
    });
  }

  // loading indices
  for (int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace& face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // loading material
  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture*> loaded_textures;
    material.diffuse = _LoadMaterialTextures(mat, aiTextureType_DIFFUSE, &loaded_textures);
    material.specular = _LoadMaterialTextures(mat, aiTextureType_SPECULAR, &loaded_textures);
    material.shininess = 32;
  }

  return Mesh(vertices, indices, material);
}

std::vector<Texture*> Model::_LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::vector<Texture*>* loaded_textures)
{
  std::vector<Texture*> textures;

  printf("texture_count: %u\n", material->GetTextureCount(type));
  for (int i = 0; i < material->GetTextureCount(type); i++)
  {
    aiString name;
    material->GetTexture(type, i, &name);
    std::string path = std::string(std::string(_directory) + "/" + name.C_Str());

    bool skip_texture = false;
    for (Texture* texture : *loaded_textures)
    {
      if (path == texture->GetPath())
      {
        skip_texture = true;
        break;
      }
    }

    if (!skip_texture)
    {
      Texture* texture = new Texture(path.c_str(), true);
      textures.push_back(texture);
      loaded_textures->push_back(texture);
    }
  }

  return textures;
}
