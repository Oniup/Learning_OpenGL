#include <app/application.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <vector>
#include <string>

#include <app/graphics_types.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <app/transform.hpp>
#include <app/light.hpp>
#include <app/imgui_handler.hpp>

static Camera camera;
static bool cursor_mode = true;

void mouse_callback(GLFWwindow* window, double x, double y);

Application::Application()
{
  _init_window();
  init_imgui(this);
}

Application::~Application()
{
  terminate_imgui();

  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Application::run()
{
  // glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  camera.position = glm::vec3(0.0f, 0.0f, 20.0f);
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
  std::vector<Light> lights;
  for (int i = 0; i < 2; i++)
  {
    Light light;
    light.transform.position = glm::vec3(0.0f, 3.0f, 3.0f);
    light.transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
    light.transform.rotation = glm::vec3(0.0f, 0.0f, 1.0f);
    light.transform.angle = 0.0f;

    light.type = LIGHT_TYPE_POINT;
    light.colour = glm::vec4(1.0f, 0.9f, 0.9f, 1.0f);
    lights.push_back(light);
  }

  Light* directional = &lights[0];
  directional->transform.position = glm::vec3(0.0f, 5.0f, 5.0f);
  directional->type = LIGHT_TYPE_DIRECTIONAL;

  float ambient_strength = 0.5f;
  glm::vec4 ambient_colour = glm::vec4(0.0f, 0.05f, 0.25f, 1.0f);

  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), (float)window_width() / (float)window_height(), 
    0.1f, 100.0f
  );

  while (!glfwWindowShouldClose(_window))
  {
    glClearColor(ambient_colour.r, ambient_colour.g, ambient_colour.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (glfwGetKey(_window, GLFW_KEY_L) == GLFW_PRESS && !cursor_mode)
    {
      cursor_mode = true;
      glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetCursorPosCallback(_window, nullptr);
    }
    else if (glfwGetKey(_window, GLFW_KEY_P) == GLFW_PRESS && cursor_mode)
    {
      cursor_mode = false;
      glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPosCallback(_window, mouse_callback);
    }

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

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, objects[i].position);
      model = glm::scale(model, objects[i].scale);
      model = glm::rotate(model, glm::radians(objects[i].angle), objects[i].rotation);

      glm::mat4 view_model = view * model;

      uint32_t u_projection = glGetUniformLocation(shader.id(), "u_projection");
      uint32_t u_view_model = glGetUniformLocation(shader.id(), "u_view_model");
      glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
      glUniformMatrix4fv(u_view_model, 1, GL_FALSE, &view_model[0][0]);

      uint32_t u_texture = glGetUniformLocation(shader.id(), "u_texture");
      glUniform1i(u_texture, 0);

      for (int i = 0; i < lights.size(); i++)
      {
        std::string name = "u_lights[" + std::to_string(i) + "].";

        uint32_t u_light_position = glGetUniformLocation(shader.id(), std::string(name + "position").c_str());
        uint32_t u_light_colour = glGetUniformLocation(shader.id(), std::string(name + "colour").c_str());
        uint32_t u_lights_count = glGetUniformLocation(shader.id(), "u_lights_count");
        uint32_t u_ambient_colour = glGetUniformLocation(shader.id(), "u_ambient_colour");

        glm::vec3 position = glm::vec3(view * glm::vec4(lights[i].transform.position, 1.0f));

        glUniform3f(u_light_position,
          position.x, position.y, position.z
        );
        glUniform3f(u_light_colour, 
          lights[i].colour.r, lights[i].colour.g, lights[i].colour.b
        );
        glUniform1i(u_lights_count, lights.size());
        glUniform4f(u_ambient_colour,
          ambient_colour.r, ambient_colour.g, ambient_colour.b, ambient_colour.a
        );
      }

      glDrawArrays(GL_TRIANGLES, 0, 36);

      crate_texture.unbind();
    }
    shader.unbind();
    glBindVertexArray(0);

    render_lights(lights, &light_shader, &camera, projection, view);

    start_imgui();
    light_controller_imgui(ambient_colour, lights);
    end_imgui();

    // ImGui Window Panel Stuff

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
  if (!cursor_mode)
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
}