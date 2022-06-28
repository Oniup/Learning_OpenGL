#include <app/light.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

static uint32_t vao, vbo;

void generate_light_vertex_data()
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

void clean_light_vertex_data()
{
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void rotate_light_around_target(Light* light, const Transform& target, float radius)
{
  float time = (float)glfwGetTime();

  light->transform.position = glm::vec3(
    cosf(time) * radius,
    light->transform.position.y,
    sinf(time) * radius
  );
}

void render_lights(const Light* lights, const int light_count, Shader* shader, const Camera* camera, const glm::mat4& projection, const glm::mat4& view)
{
  glBindVertexArray(vao);
  shader->bind();
  for (int i = 0; i < light_count; i++)
  {
    const Light* light = &lights[i];

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, light->transform.position);
    transform = glm::scale(transform, light->transform.scale);
    transform = glm::rotate(transform, glm::radians(light->transform.angle), light->transform.rotation);

    uint32_t crate_transform_location = glGetUniformLocation(shader->id(), "transform");
    uint32_t projection_location = glGetUniformLocation(shader->id(), "projection");
    uint32_t view_location = glGetUniformLocation(shader->id(), "view");

    uint32_t light_colour_location = glGetUniformLocation(shader->id(), "light_colour");

    glUniformMatrix4fv(crate_transform_location, 1, GL_FALSE, &transform[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);
    glUniform4f(light_colour_location, light->colour.r, light->colour.g, light->colour.b, light->colour.a);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  shader->unbind();
  glBindVertexArray(0);
}
