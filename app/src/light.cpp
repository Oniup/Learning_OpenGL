#include <app/light.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

static uint32_t vao, vbo;

void GenerateLightVertexData()
{
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
  glBindVertexArray(0);

}

void CleanLightVertexData()
{
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void RotateLightAroundTarget(Light* light, const Transform& target, float radius)
{
  float time = (float)glfwGetTime();

  light->transform.position = glm::vec3(
    cosf(time) * radius,
    light->transform.position.y,
    sinf(time) * radius
  );
}

void RenderLights(const std::vector<Light>& lights, Shader* shader, const Camera* camera, const glm::mat4& projection, const glm::mat4& view)
{
  glBindVertexArray(vao);
  shader->Bind();
  glm::mat4 projection_view = projection * view;
  for (int i = 0; i < lights.size(); i++)
  {
    const Light* light = &lights[i];

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, light->transform.position);
    model = glm::scale(model, light->transform.scale);
    model = glm::rotate(model, glm::radians(light->transform.angle), light->transform.rotation);

    glm::mat4 final_matrix = projection_view * model;
    
    uint32_t u_final_matrix = glGetUniformLocation(shader->id(), "u_final_matrix");
    uint32_t u_light_colour = glGetUniformLocation(shader->id(), "u_light_colour");

    glm::vec3 light_colour = light->diffuse + light->specular + light->ambient;

    glUniformMatrix4fv(u_final_matrix, 1, GL_FALSE, &final_matrix[0][0]);
    glUniform3f(u_light_colour, light_colour.r, light_colour.g, light_colour.b);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  shader->Unbind();
  glBindVertexArray(0);
}
