#include "app/model.hpp"

#include "app/utils.hpp"

Model::Model()
    : transform(), m_meshes(), m_textures(), m_shader(nullptr), m_directory("")
{
}

Model::Model(const Model& model)
    : transform(model.transform), m_meshes(model.m_meshes), m_textures(), m_shader(model.m_shader), m_directory(model.m_directory)
{
}

Model::Model(Model&& model)
    : transform(static_cast<Transform&&>(model.transform)), m_meshes(static_cast<std::vector<Mesh*>&&>(model.m_meshes)),
      m_textures(static_cast<std::vector<Texture*>&&>(model.m_textures)), 
      m_shader(model.m_shader), m_directory(static_cast<std::string&&>(model.m_directory))
{
    model.m_shader = nullptr;
}

Model::Model(const char* path)
    : Model()
{
    m_LoadModel(path);
}

Model::~Model()
{
    Clear();
}

Model& Model::operator=(const Model& model)
{
    Clear();

    transform = model.transform;
    m_meshes = model.m_meshes;
    m_textures = model.m_textures;
    m_shader = model.m_shader;
    m_directory = model.m_directory;

    return *this;
}

Model& Model::operator=(Model&& model)
{
    Clear();

    transform = static_cast<Transform&&>(model.transform);
    m_meshes = static_cast<std::vector<Mesh*>&&>(model.m_meshes);
    m_textures = static_cast<std::vector<Texture*>&&>(model.m_textures);
    m_shader = model.m_shader;
    m_directory = static_cast<std::string&&>(model.m_directory);

    model.m_shader = nullptr;

    return *this;
}

void Model::Render(const glm::mat4& view, const glm::mat4& projection)
{
    for (Mesh* mesh : m_meshes)
        mesh->Render(m_shader, view, projection, transform);
}

void Model::Clear()
{
    for (Mesh* mesh : m_meshes)
        delete mesh;

    for (Texture* texture : m_textures)
        delete texture;
    
    m_textures.clear();
    m_meshes.clear();
    m_directory.clear();
    m_shader = nullptr;
    transform.position = glm::vec3(0.0f);
    transform.scale = glm::vec3(0.0f);
    transform.rotation = glm::vec3(0.0f);
    transform.angle = 0.0f;
}

void Model::SetShader(Shader* shader)
{
    m_shader = shader;
}

void Model::m_LoadModel(const char* path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(std::string(path), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene)
    {
        printf("failed to load model\nassimp error: %s\n", importer.GetErrorString());
        exit(-1);
    }

    m_directory = utils::GetPathDirectory(path);

    m_ProcessNode(scene->mRootNode, scene);
}

void Model::m_ProcessNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(m_ProcessMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; i++)
        m_ProcessNode(node->mChildren[i], scene);
}

Mesh* Model::m_ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<uint32_t> indices;

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.push_back({
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y),
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
        });
    }

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace* face = &mesh->mFaces[i];
        for (int j = 0; j < face->mNumIndices; j++)
            indices.push_back(face->mIndices[j]);
    }
    
    // TODO: get the diffuse and specular texture maps 

    Mesh* result = new Mesh(vertices, indices);
    return result;
}

void Model::m_LoadTextures(aiMaterial* material, aiTextureType type)
{
    // TODO: load textures
}

