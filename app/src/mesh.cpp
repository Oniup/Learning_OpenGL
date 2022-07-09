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
  assert(shader && "failed to render mesh as the shader is set to nullptr");

  uint32_t u_shininess = glGetUniformLocation(shader->id(), "u_material.shininess");
  glUniform1i(u_shininess, material.shininess);
   
  int texture_number = 0; 
  for (int i = 0; i < 2; i++)
  {
    std::string name;
    std::vector<Texture>* textures = nullptr;

    if (i == 0)
    {
      name = "u_material.diffuse[";
      textures = &material.diffuse;
    }
    else
    {
      name = "u_material.specular[";
      textures = &material.specular;
    }

    for (Texture& texture : *textures)
    {
      texture.Bind(texture_number);
      uint32_t u_diffuse = glGetUniformLocation(shader->id(), std::string(name + std::to_string(i) + "]").c_str());
      glUniform1i(u_diffuse, texture_number);

      texture_number++;
    }
  }

  glm::mat4 model_matrix = glm::mat4(0.0f);
  model_matrix = glm::translate(model_matrix, transform->position);
  model_matrix = glm::scale(model_matrix, transform->scale);
  model_matrix = glm::rotate(model_matrix, glm::radians(transform->angle), transform->rotation);
  glm::mat4 view_model = view * model_matrix;

  uint32_t u_view_model = glGetUniformLocation(shader->id(), "u_view_model");
  uint32_t u_projection = glGetUniformLocation(shader->id(), "u_projection");
  glUniformMatrix4fv(u_view_model, 1, GL_FALSE, &view_model[0][0]);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
}

void Mesh::_InitVertexData()
{
  _Clear();

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glGenBuffers(1, &_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), vertices.data(), GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0); // a_vertex_pos
  glEnableVertexAttribArray(1); // a_uv
  glEnableVertexAttribArray(2); // a_normals

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

