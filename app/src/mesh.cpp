#include "app/mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

Mesh::Mesh()
    : m_vertices(), m_indices(), m_diffuse_textures(), m_specular_textures(), m_vao(0), m_vbo(0), m_ebo(0)
{
}

Mesh::Mesh(const Mesh& mesh)
    : m_vertices(mesh.m_vertices), m_indices(mesh.m_indices), 
      m_diffuse_textures(mesh.m_diffuse_textures), m_specular_textures(mesh.m_specular_textures), 
      m_vao(0), m_vbo(0), m_ebo(0)
{
    m_GenerateVertexData();
}

Mesh::Mesh(Mesh&& mesh)
    : m_vertices(static_cast<std::vector<Vertex>&&>(mesh.m_vertices)), m_indices(static_cast<std::vector<uint32_t>&&>(mesh.m_indices)),
      m_diffuse_textures(static_cast<std::vector<Texture*>&&>(mesh.m_diffuse_textures)), 
      m_specular_textures(static_cast<std::vector<Texture*>&&>(mesh.m_specular_textures)), 
      m_vao(mesh.m_vao), m_vbo(mesh.m_vbo), m_ebo(mesh.m_ebo)
{
    mesh.m_vao = 0;
    mesh.m_vbo = 0;
    mesh.m_ebo = 0;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    : m_vertices(vertices), m_indices(indices), m_diffuse_textures(), m_specular_textures(),
      m_vao(0), m_vbo(0), m_ebo(0)
{
    m_GenerateVertexData();
}

Mesh::~Mesh()
{
    Clear();
}

Mesh& Mesh::operator=(const Mesh& mesh)
{
    Clear();

    m_vertices = mesh.m_vertices;
    m_indices = mesh.m_indices;
    m_diffuse_textures = mesh.m_diffuse_textures;
    m_specular_textures = mesh.m_specular_textures;

    m_GenerateVertexData();

    return *this;
}

Mesh& Mesh::operator=(Mesh&& mesh)
{
    Clear();

    m_vertices = static_cast<std::vector<Vertex>&&>(mesh.m_vertices);
    m_indices = static_cast<std::vector<uint32_t>&&>(mesh.m_indices);
    m_diffuse_textures = static_cast<std::vector<Texture*>&&>(mesh.m_diffuse_textures);
    m_specular_textures = static_cast<std::vector<Texture*>&&>(mesh.m_specular_textures);

    m_vao = mesh.m_vao;
    m_vbo = mesh.m_vbo;
    m_ebo = mesh.m_ebo;

    mesh.m_vao = 0;
    mesh.m_vbo = 0;
    mesh.m_ebo = 0;

    return *this;
}

void Mesh::Render(Shader* shader, const glm::mat4& view, const glm::mat4& projection, const Transform& transform)
{
    shader->Bind();

    // TODO: implement texture mapping here ...

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::scale(model, transform.scale);
    model = glm::rotate(model, glm::radians(transform.angle), transform.rotation);

    glm::mat4 view_model = view * model;

    uint32_t u_view_model = glGetUniformLocation(shader->Id(), "u_view_model");
    uint32_t u_projection = glGetUniformLocation(shader->Id(), "u_projection");
    glUniformMatrix4fv(u_view_model, 1, GL_FALSE, &view_model[0][0]);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    Shader::Unbind();
}

void Mesh::Clear()
{
    if (m_vao > 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);

        m_vao = 0;
        m_vbo = 0;
        m_ebo = 0;
    }

    m_vertices.clear();
    m_indices.clear();
    m_diffuse_textures.clear();
    m_specular_textures.clear();
}

void Mesh::m_GenerateVertexData()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // vertex_pos
    glEnableVertexAttribArray(1); // uv
    glEnableVertexAttribArray(2); // normal

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
}
