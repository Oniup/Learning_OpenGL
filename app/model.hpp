#ifndef MODEL_HPP
#define MODEL_HPP

#include "app/mesh.hpp"
#include "app/graphics_types.hpp"
#include "app/transform.hpp"

#include <stdio.h>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Transform transform;

public:
    Model();
    Model(const Model& model);
    Model(Model&& model);
    Model(const char* path);
    ~Model();

    Model& operator=(const Model& model);
    Model& operator=(Model&& model);

public:
    void Render(const glm::mat4& view, const glm::mat4& projection);
    void Clear();
    void SetShader(Shader* shader);

private:
    std::vector<Mesh*> m_meshes;
    std::vector<Texture*> m_textures;
    Shader* m_shader;
    std::string m_directory;

private:
    void m_LoadModel(const char* path);
    void m_ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* m_ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void m_LoadTextures(aiMaterial* material, aiTextureType type);

};

#endif // MODEL_HPP