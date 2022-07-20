#include "app/light.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

static uint32_t _s_vao, _s_ebo, _s_vbo;

void GenerateLightVertexData()
{
    float vertices[] = {
        -1.0f, -1.0f,    1.0f,
         1.0f, -1.0f,    1.0f,
         1.0f,  1.0f,    1.0f,
        -1.0f,  1.0f,    1.0f,

        -1.0f, -1.0f,   -1.0f,
        -1.0f,  1.0f,   -1.0f,
         1.0f,  1.0f,   -1.0f,
         1.0f, -1.0f,   -1.0f,

        -1.0f,  1.0f,   -1.0f,
        -1.0f,  1.0f,    1.0f,
         1.0f,  1.0f,    1.0f,
         1.0f,  1.0f,   -1.0f,

        -1.0f, -1.0f,   -1.0f,
         1.0f, -1.0f,   -1.0f,
         1.0f, -1.0f,    1.0f,
        -1.0f, -1.0f,    1.0f,
        
         1.0f, -1.0f,   -1.0f,
         1.0f,  1.0f,   -1.0f,
         1.0f,  1.0f,    1.0f,
         1.0f, -1.0f,    1.0f,

        -1.0f, -1.0f,   -1.0f,
        -1.0f, -1.0f,    1.0f,
        -1.0f,  1.0f,    1.0f,
        -1.0f,  1.0f,   -1.0f
    };
    uint32_t indices[] = {    // note that we start from 0!
        0,   1,  2,   0, 2,   3,     // front
        4,   5,  6,   4, 6,   7,     // back
        8,   9,  10,  8, 10, 11,     // top
        12, 13, 14,  12, 14, 15,     // bottom
        16, 17, 18,  16, 18, 19,     // right
        20, 21, 22,  20, 22, 23,     // left
    };

    glGenVertexArrays(1, &_s_vao);
    glBindVertexArray(_s_vao);

    glGenBuffers(1, &_s_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _s_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &_s_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindVertexArray(0);
}

void CleanLightVertexData()
{
    glDeleteBuffers(1, &_s_vbo);
    glDeleteVertexArrays(1, &_s_vao);
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
    glBindVertexArray(_s_vao);
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
        
        uint32_t u_final_matrix = glGetUniformLocation(shader->Id(), "u_final_matrix");
        uint32_t u_light_colour = glGetUniformLocation(shader->Id(), "u_light_colour");

        glm::vec3 light_colour = light->diffuse + light->specular + light->ambient;

        glUniformMatrix4fv(u_final_matrix, 1, GL_FALSE, &final_matrix[0][0]);
        glUniform3f(u_light_colour, light_colour.r, light_colour.g, light_colour.b);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    }

    Shader::Unbind();
    glBindVertexArray(0);
}
