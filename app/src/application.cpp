#include <app/application.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <vector>

#include <app/graphics_types.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>


static Camera camera;

void mouse_callback(GLFWwindow* window, double x, double y);


Application::Application()
{
  _init_window();
}

Application::~Application()
{
  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Application::run()
{
  Shader shader("app/assets/shaders/basic.vert", "app/assets/shaders/basic.frag");
  Texture texture("app/assets/images/crate.png", false);


  float vertices[] = {
  // vertex data          uv           normals
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, 
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, 
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 

    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
  };

  uint32_t vao, vbo;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0); // vertex position
  glEnableVertexAttribArray(1); // uv texture coords
  glEnableVertexAttribArray(2); // normal vectors
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 5));
  glBindVertexArray(0);

  camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
  camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
  camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);

  Transform crate;
  crate.position = glm::vec3(0.0f, 0.0f, 0.0f);
  crate.scale = glm::vec3(1.0f, 1.0f, 1.0f);
  crate.rotation = glm::vec3(1.0f, 0.0f, 0.0f);
  crate.angle = 0.0f;

  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), (float)window_width() / (float)window_height(), 
    0.1f, 100.0f
  );

  while (!glfwWindowShouldClose(_window))
  {
    glClearColor(0.2f, 0.6f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.bind();
    texture.bind(0);
    glBindVertexArray(vao);

    /************ UPDATING ***********/    

    float time = (float)glfwGetTime();
    static float last_time = 0.0f;
    float delta_time = time - last_time;
    last_time = time;

    crate.angle += delta_time * 20.0f;
    if (crate.angle > 360.0f)
      crate.angle = 0.0f;

    _camera_controller(camera, delta_time);

    /*********** RENDERING ***********/

    glm::mat4 crate_transform = glm::mat4(1.0f);
    crate_transform = glm::translate(crate_transform, crate.position);
    crate_transform = glm::scale(crate_transform, crate.scale);
    crate_transform = glm::rotate(crate_transform, glm::radians(crate.angle), crate.rotation);

    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);

    uint32_t texture_location = glGetUniformLocation(shader.id(), "texture_sample");
    uint32_t crate_transform_location = glGetUniformLocation(shader.id(), "transform");
    uint32_t projection_location = glGetUniformLocation(shader.id(), "projection");
    uint32_t view_location = glGetUniformLocation(shader.id(), "view");

    glUniform1i(texture_location, 0);
    glUniformMatrix4fv(crate_transform_location, 1, GL_FALSE, &crate_transform[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    shader.unbind();
    texture.unbind();
    glBindVertexArray(0);

    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}

void Application::_init_window()
{
  assert(glfwInit() && "failed to initialise glfw for some reason ...");

  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);

#ifdef __APPLE__
  // I have no clue what this is doing, I just know you have to add this if on Mac 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

  _window = glfwCreateWindow(window_width(), window_height(), "OpenGL Window", nullptr, nullptr);
  assert(_window && "failed to initialise window for some reason ...");

  glfwMakeContextCurrent(_window);
  assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && "failed to initialise glad for some reason ...");

  glViewport(0, 0, window_width(), window_height());
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
}

void Application::_camera_controller(Camera& camera, float delta_time)
{
  float move_speed = 5.0f;

  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    camera.position -= glm::cross(camera.forward, camera.up) * move_speed * delta_time;
  if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    camera.position += glm::cross(camera.forward, camera.up) * move_speed * delta_time;
  if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
  {
    glm::vec3 right = glm::cross(camera.forward, camera.up);
    camera.position += glm::cross(camera.forward, right) * move_speed * delta_time;
  }
  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
  {
    glm::vec3 right = glm::cross(camera.forward, camera.up);
    camera.position -= glm::cross(camera.forward, right) * move_speed * delta_time;
  }
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
  
}