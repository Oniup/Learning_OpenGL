#ifndef MESH_HPP
#define MESH_HPP

#include "app/graphics_types.hpp"
#include "app/transform.hpp"

#include <glm/glm.hpp>

#include <stdio.h>
#include <stdint.h>
#include <vector>

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
    };

public:
    Mesh();
    Mesh(const Mesh& mesh);
    Mesh(Mesh&& mesh);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~Mesh();

    Mesh& operator=(const Mesh& mesh);
    Mesh& operator=(Mesh&& mesh);

public:
    void Render(Shader* shader, const glm::mat4& view, const glm::mat4& projection, const Transform& transform);
    void Clear();

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture*> m_diffuse_textures;
    std::vector<Texture*> m_specular_textures;

    uint32_t m_vao, m_vbo, m_ebo;

private:
    void m_GenerateVertexData();

};

#endif // MESH_HPP