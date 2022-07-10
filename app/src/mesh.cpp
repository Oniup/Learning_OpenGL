#include <app/mesh.hpp>

#include <stdio.h>
#include <assert.h>

#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <app/graphics_types.hpp>
#include <app/light.hpp>
#include <app/transform.hpp>

Mesh::Mesh() : vertices(), indices(), material(), _vao(0), _vbo(0), _ebo(0)
{
  material.shininess = 0.0f;
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const Material& material) 
  : vertices(), indices(), material(), _vao(0), _vbo(0), _ebo(0)
{
  this->vertices = vertices;
  this->indices = indices;
  this->material = material;

  _InitVertexData();
}

Mesh::~Mesh()
{
  _Clear();
}

void Mesh::Render(Shader* shader, const glm::mat4& view, const glm::mat4& projection, const std::vector<Light>& lights, const Transform* transform)
{
  shader->Bind();

  int texture_offset = 0;
  for (int i = 0; i < 2; i++)
  {
    std::string name = "";
    std::vector<Texture*>* textures = nullptr;

    if (i == 0)
    {
      name = "u_material.diffuse[";
      uint32_t u_diffuse_count = glGetUniformLocation(shader->Id(), "u_material.diffuse_count");
      glUniform1i(u_diffuse_count, material.diffuse.size());
      textures = &material.diffuse;
    }
    else
    {
      name = "u_material.specular[";
      uint32_t u_specular_count = glGetUniformLocation(shader->Id(), "u_material.specular_count");
      glUniform1i(u_specular_count, material.specular.size());
      textures = &material.specular;
    }

    for (Texture* texture : *textures)
    {
      uint32_t u_location = glGetUniformLocation(shader->Id(), std::string(name + std::to_string(i) + "]").c_str());
      texture->Bind(texture_offset);
      glUniform1i(u_location, texture_offset);
      texture_offset++;
    }
  }

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, transform->position);
  model = glm::scale(model, transform->scale);
  model = glm::rotate(model, glm::radians(transform->angle), transform->rotation);

  glm::mat4 view_model = view * model;

  uint32_t u_view_model = glGetUniformLocation(shader->Id(), "u_view_model");
  uint32_t u_projection = glGetUniformLocation(shader->Id(), "u_projection");
  glUniformMatrix4fv(u_view_model, 1, GL_FALSE, &view_model[0][0]);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);

  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);

  shader->Unbind();
}

void Mesh::_InitVertexData()
{
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glGenBuffers(1, &_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0); // a_vertex_pos
  glEnableVertexAttribArray(1); // a_uv
  glEnableVertexAttribArray(2); // a_normal

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, uv));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));

  glBindVertexArray(0);
}

void Mesh::_Clear()
{
  vertices.clear();
  indices.clear();

  material.diffuse.clear();
  material.specular.clear();
  material.shininess = 0.0f;


  if (_vao != 0)
  {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);

    _vao = 0;
    _vbo = 0;
    _ebo = 0;
  }  
}

