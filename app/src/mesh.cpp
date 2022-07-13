#include <app/mesh.hpp>

#include <app/graphics_types.hpp>
#include <app/transform.hpp>
#include <app/light.hpp>
#include <app/camera.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh() 
  : _material({ std::vector<Texture*>(), std::vector<Texture*>(), 0 }), _vertices(), _indices(), _vao(0), _vbo(0), _ebo(0)
{
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> indices, const Material& material, bool init_vertex_data /* = true */)
  : _material({ material.diffuse, material.specular, material.shininess }), _vertices(vertices), _indices(indices), _vao(0), _vbo(0), _ebo(0)
{
  if (init_vertex_data)
    _InitVertexData();
}

Mesh::~Mesh()
{
  Free();
}

Mesh& Mesh::operator=(const Mesh& mesh)
{
  Free();

  _material = mesh._material;
  _vertices = mesh._vertices;
  _indices = mesh._indices;

  _InitVertexData();

  return *this;
}

Mesh* Mesh::GenerateCube(const Material& material)
{
  std::vector<Vertex> vertices = {
  // vertex data          uv           normals
    { glm::vec3( 1.0f, -1.0f, -1.0f),  glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f) },
    { glm::vec3( 1.0f,  1.0f, -1.0f),  glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f) },
    { glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f) },
    { glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f) },

    { glm::vec3( 1.0f, -1.0f,  1.0f),  glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f) },
    { glm::vec3( 1.0f,  1.0f,  1.0f),  glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f,  1.0f) },
    { glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  0.0f,  1.0f) },
    { glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f) },

    { glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f) },
    { glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f) },
    { glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f) },
    { glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f) },

    { glm::vec3( 1.0f,  1.0f, -1.0f),  glm::vec2(1.0f, 1.0f), glm::vec3( 1.0f,  0.0f,  0.0f) },
    { glm::vec3( 1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 1.0f), glm::vec3( 1.0f,  0.0f,  0.0f) },
    { glm::vec3( 1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f) },
    { glm::vec3( 1.0f,  1.0f,  1.0f),  glm::vec2(1.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f) },

    { glm::vec3( 1.0f, -1.0f, -1.0f),  glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, -1.0f,  0.0f) },
    { glm::vec3( 1.0f, -1.0f,  1.0f),  glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f) },
    { glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f) },
    { glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, -1.0f,  0.0f) },

    { glm::vec3( 1.0f,  1.0f, -1.0f),  glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  1.0f,  0.0f) },
    { glm::vec3( 1.0f,  1.0f,  1.0f),  glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f) },
    { glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f) },
    { glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  1.0f,  0.0f) }
  };

  std::vector<uint32_t> indices = {
    3,  2,  0,  2,  0,  1,
    7,  6,  4,  6,  4,  5,
    11, 10, 8,  10, 8,  9,
    15, 14, 12, 14, 12, 13,
    19, 18, 16, 18, 16, 17,
    23, 22, 20, 22, 20, 21
  };  

  Mesh* mesh = new Mesh(vertices, indices, material);
  return mesh;
}

void Mesh::Render(Shader* shader, const Transform& transform, const std::vector<Light>& lights, const glm::mat4& view, const glm::mat4& projection)
{
  shader->Bind();

  // geometry manipulations
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, transform.position);
  model = glm::scale(model, transform.scale);
  model = glm::rotate(model, glm::radians(transform.angle), transform.rotation);
  glm::mat4 view_model = view * model;

  uint32_t u_view_model = glGetUniformLocation(shader->Id(), "u_view_model");
  uint32_t u_projection = glGetUniformLocation(shader->Id(), "u_projection");  
  glUniformMatrix4fv(u_view_model, 1, GL_FALSE, &view_model[0][0]);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);

  // material
  uint32_t u_shininess = glGetUniformLocation(shader->Id(), "u_material.shininess");
  glUniform1i(u_shininess, _material.shininess);

  int texture_offset = 0;
  for (int i = 0; i < 2; i++)
  {
    std::string texture_name;
    std::vector<Texture*>* textures = nullptr;
    uint32_t u_count_location = 0;

    switch (i)
    {
      case 0:
        texture_name = "u_material.diffuse[";
        textures = &_material.diffuse;
        u_count_location = glGetUniformLocation(shader->Id(), "u_material.diffuse_count");
        break;
      case 1:
        texture_name = "u_material.specular[";
        textures = &_material.specular;
        u_count_location = glGetUniformLocation(shader->Id(), "u_material.specular_count");
        
        break;
    }

    glUniform1i(u_count_location, textures->size());

    for (int j = 0; j < textures->size(); j++)
    {
      std::string name = texture_name + std::to_string(j) + "]";
      uint32_t u_location = glGetUniformLocation(shader->Id(), name.c_str());
      textures->at(j)->Bind(texture_offset);
      glUniform1i(u_location, texture_offset);

      texture_offset++;
    }
  }

  // lighting
  uint32_t u_lights_count = glGetUniformLocation(shader->Id(), "u_lights_count");
  glUniform1i(u_lights_count, lights.size());

  for (int i = 0; i < lights.size(); i++)
  {
    std::string name = "u_lights[" + std::to_string(i) + "].";

    uint32_t u_type = glGetUniformLocation(shader->Id(), std::string(name + "type").c_str());
    uint32_t u_position = glGetUniformLocation(shader->Id(), std::string(name + "position").c_str());
    uint32_t u_direction = glGetUniformLocation(shader->Id(), std::string(name + "direction").c_str());
    uint32_t u_ambient = glGetUniformLocation(shader->Id(), std::string(name + "ambient").c_str());
    uint32_t u_diffuse = glGetUniformLocation(shader->Id(), std::string(name + "diffuse").c_str());
    uint32_t u_specular = glGetUniformLocation(shader->Id(), std::string(name + "specular").c_str());
    uint32_t u_constant = glGetUniformLocation(shader->Id(), std::string(name + "constant").c_str());
    uint32_t u_linear = glGetUniformLocation(shader->Id(), std::string(name + "linear").c_str());
    uint32_t u_quadratic = glGetUniformLocation(shader->Id(), std::string(name + "quadratic").c_str());
    uint32_t u_spot_start_fade = glGetUniformLocation(shader->Id(), std::string(name + "spot_start_fade").c_str());
    uint32_t u_spot_end_fade = glGetUniformLocation(shader->Id(), std::string(name + "spot_end_fade").c_str());

    glm::vec3 position = glm::vec3(view * glm::vec4(lights[i].transform.position, 1.0f));
    glm::vec3 direction = glm::vec3(glm::transpose(glm::inverse(view)) * glm::vec4(lights[i].direction, 1.0f));

    glUniform1i(u_type, lights[i].type);
    glUniform3fv(u_position, 1, &position[0]);
    glUniform3fv(u_direction, 1, &direction[0]);
    glUniform3fv(u_ambient, 1, &lights[i].ambient[0]);
    glUniform3fv(u_diffuse, 1, &lights[i].diffuse[0]);
    glUniform3fv(u_specular, 1, &lights[i].specular[0]);
    glUniform1f(u_constant, lights[i].constant);
    glUniform1f(u_linear, lights[i].linear);
    glUniform1f(u_quadratic, lights[i].quadratic);
    glUniform1f(u_spot_start_fade, lights[i].spot_start_fade);
    glUniform1f(u_spot_end_fade, lights[i].spot_end_fade);
  }

  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);

  Shader::Unbind();
  Texture::Unbind();
}

void Mesh::Free()
{
  if (_vao != 0)
  {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);

    _vbo = 0;
    _ebo = 0;
    _vao = 0;
  }

  _material.diffuse.clear();
  _material.specular.clear();
  _material.shininess = 0;

  _vertices.clear();
  _indices.clear();
}

void Mesh::_InitVertexData()
{
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glGenBuffers(1, &_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0); // a_vertex_pos
  glEnableVertexAttribArray(1); // a_uv
  glEnableVertexAttribArray(2); // a_normal

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glBindVertexArray(0);
}
