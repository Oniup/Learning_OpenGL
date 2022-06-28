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

#include <app/transform.hpp>
#include <app/light.hpp>


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
  glfwSetCursorPosCallback(_window, mouse_callback);
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Shader shader("app/assets/shaders/basic.vert", "app/assets/shaders/basic.frag");
  Shader light_shader("app/assets/shaders/light.vert", "app/assets/shaders/light.frag");

  Texture crate_texture("app/assets/images/crate.png", false);
  Texture crate_specular_texture("app/assets/images/crate_specular.png", false);

  Texture floor_texture("app/assets/images/floor.png", false);
  Texture floor_specular_texture("app/assets/images/floor_specular.png", false);



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

  generate_light_vertex_data();

  camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
  camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
  camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);

  static const int objects_count = 2;
  Transform objects[objects_count];

  objects[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
  objects[0].scale = glm::vec3(1.0f, 1.0f, 1.0f);
  objects[0].rotation = glm::vec3(1.0f, 0.0f, 1.0f);
  objects[0].angle = 0.0f;

  objects[1].position = glm::vec3(0.0f, -3.0f, 0.0f);
  objects[1].scale = glm::vec3(5.0f, 0.2f, 5.0f);
  objects[1].rotation = glm::vec3(0.0f, 0.0f, 1.0f);
  objects[1].angle = 0.0f;

  static const int light_count = 2;
  Light lights[light_count];
  for (int i = 0; i < light_count; i++)
  {
    Light* light = &lights[i];
    light->transform.position = glm::vec3(0.0f, 3.0f, 3.0f);
    light->transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
    light->transform.rotation = glm::vec3(0.0f, 0.0f, 1.0f);
    light->transform.angle = 0.0f;

    light->type = LIGHT_TYPE_POINT;
    light->colour = glm::vec4(1.0f, 0.9f, 0.9f, 1.0f);
  }

  Light* directional = &lights[0];
  directional->transform.position = glm::vec3(0.0f, 5.0f, 5.0f);
  directional->type = LIGHT_TYPE_DIRECTIONAL;

  float ambient_strength = 0.5f;
  glm::vec3 ambient_colour = glm::vec3(0.0f, 0.05f, 0.25f);

  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), (float)window_width() / (float)window_height(), 
    0.1f, 100.0f
  );

  while (!glfwWindowShouldClose(_window))
  {
    glClearColor(ambient_colour.r, ambient_colour.g, ambient_colour.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.bind();
    glBindVertexArray(vao);

    /************ UPDATING ***********/    

    float time = (float)glfwGetTime();
    static float last_time = 0.0f;
    float delta_time = time - last_time;
    last_time = time;

    objects[0].angle += delta_time * 20.0f;
    if (objects[0].angle > 360.0f)
      objects[0].angle = 0.0f;

    _camera_controller(camera, delta_time);
    rotate_light_around_target(&lights[1], objects[0], 10.0f);

    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);

    /*********** RENDERING ***********/

    for (int i = 0; i < objects_count; i++)
    {
      if (i == 0)
        crate_texture.bind(0);
      else
        floor_texture.bind(0);

      glm::mat4 transform = glm::mat4(1.0f);
      transform = glm::translate(transform, objects[i].position);
      transform = glm::scale(transform, objects[i].scale);
      transform = glm::rotate(transform, glm::radians(objects[i].angle), objects[i].rotation);

      uint32_t texture_location = glGetUniformLocation(shader.id(), "texture_sample");
      uint32_t crate_transform_location = glGetUniformLocation(shader.id(), "transform");
      uint32_t projection_location = glGetUniformLocation(shader.id(), "projection");
      uint32_t view_location = glGetUniformLocation(shader.id(), "view");

      uint32_t ambient_colour_location = glGetUniformLocation(shader.id(), "ambient_colour");
      uint32_t ambient_strength_location = glGetUniformLocation(shader.id(), "ambient_strength");

      glUniform1i(texture_location, 0);

      glUniformMatrix4fv(crate_transform_location, 1, GL_FALSE, &transform[0][0]);
      glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);
      glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);

      // lighting
      glUniform3f(ambient_colour_location, ambient_colour.r, ambient_colour.g, ambient_colour.b);
      glUniform1f(ambient_strength_location, ambient_strength);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    shader.unbind();
    glBindVertexArray(0);

    render_lights(lights, light_count, &light_shader, &camera, projection, view);

    glfwSwapBuffers(_window);
    glfwPollEvents();
  }

  clean_light_vertex_data();

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
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
  if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    move_speed *= 2.0f;

  move_speed *= delta_time;

  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    camera.position -= glm::cross(camera.forward, camera.up) * move_speed;
  if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    camera.position += glm::cross(camera.forward, camera.up) * move_speed;
  if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    camera.position -= camera.forward * move_speed;
  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    camera.position += camera.forward * move_speed;

  if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.position.y += move_speed;
  if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    camera.position.y -= move_speed;
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
  static bool first_mouse = true;
  static float last_x = 0.0f;
  static float last_y = 0.0f;
  static float yaw = 0.0f;
  static float pitch = 0.0f;
  
  if (first_mouse)
  {
    last_x = x;
    last_y = y;
    first_mouse = false;
  }
  
  float offset_x = x - last_x;
  float offset_y = last_y - y; 
  last_x = x;
  last_y = y;

  float sensitivity = 0.05f;
  offset_x *= sensitivity;
  offset_y *= sensitivity;

  yaw   += offset_x;
  pitch += offset_y;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction = glm::vec3(
    cosf(glm::radians(yaw)) * cosf(glm::radians(pitch)),
    sinf(glm::radians(pitch)),
    sinf(glm::radians(yaw)) * cosf(glm::radians(pitch))
  );

  camera.forward = glm::normalize(direction);
}