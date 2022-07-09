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

#include <app/mesh.hpp>

static Camera camera;
static bool cursor_mode = true;

void mouse_callback(GLFWwindow* window, double x, double y);

Application::Application() : _window(nullptr), _width(1280), _height(720)
{
  _InitWindow();
  InitImGui(this);
}

Application::~Application()
{
  TerminateImGui();

  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Application::Run()
{
  bool using_phong_lighting = true;
  Shader shader("app/assets/shaders/phong.vert", "app/assets/shaders/phong.frag");
  // Shader shader("app/assets/shaders/gouraud.vert", "app/assets/shaders/gouraud.frag");
  Shader light_shader("app/assets/shaders/light.vert", "app/assets/shaders/light.frag");

  Texture crate_texture("app/assets/images/crate.png", false);
  Texture crate_specular_texture("app/assets/images/crate_specular.png", false);

  Texture floor_texture("app/assets/images/floor.png", false);
  Texture floor_specular_texture("app/assets/images/floor_specular.png", false);

  Mesh mesh;

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

  GenerateLightVertexData();

  camera.position = glm::vec3(0.0f, 0.0f, 20.0f);
  camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
  camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);

  static const int objects_count = 2;
  Transform objects[objects_count];

  objects[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
  objects[0].scale = glm::vec3(1.0f, 1.0f, 1.0f);
  objects[0].rotation = glm::vec3(1.0f, 0.0f, 1.0f);
  objects[0].angle = 0.0f;
  int object_0_shininess = 32;

  objects[1].position = glm::vec3(0.0f, -3.0f, 0.0f);
  objects[1].scale = glm::vec3(5.0f, 0.2f, 5.0f);
  objects[1].rotation = glm::vec3(0.0f, 0.0f, 1.0f);
  objects[1].angle = 0.0f;
  int object_1_shininess = 32;

  std::vector<Light> lights;
  for (int i = 0; i < 3; i++)
  {
    Light light;
    light.transform.position = glm::vec3(0.0f, 3.0f, 3.0f);
    light.transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
    light.transform.rotation = glm::vec3(0.0f, 0.0f, 1.0f);
    light.transform.angle = 0.0f;

    light.type = LIGHT_TYPE_POINT;
    light.diffuse = glm::vec3(1.0f, 0.9f, 0.9f);
    light.specular = glm::vec3(0.0f);
    light.ambient = glm::vec3(0.0f, 0.1f, 0.1f);
    light.direction = glm::vec3(0.0f);

    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;

    lights.push_back(light);
  }

  Light* directional = &lights[0];
  directional->transform.position = glm::vec3(0.0f, 5.0f, 5.0f);
  directional->direction = glm::vec3(0.5f, 0.5f, 0.5f);
  directional->type = LIGHT_TYPE_DIRECTIONAL;

  Light* camera_light = &lights[lights.size() - 1];
  camera_light->type = LIGHT_TYPE_SPOT;
  camera_light->transform.scale = glm::vec3(0.0f, 0.0f, 0.0f);
  camera_light->spot_start_fade = 12.5f;
  camera_light->spot_cutoff = 17.5f;

  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), (float)GetWindowWidth() / (float)GetWindowHeight(), 
    0.1f, 100.0f
  );

  float light_radius = 10.0f;

  glm::vec3 ambient_colour = glm::vec3(0.0f, 0.1f, 0.1f);

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

    shader.Bind();
    glBindVertexArray(vao);

    /************ UPDATING ***********/    

    float time = (float)glfwGetTime();
    static float last_time = 0.0f;
    float delta_time = time - last_time;
    last_time = time;

    objects[0].angle += delta_time * 20.0f;
    if (objects[0].angle > 360.0f)
      objects[0].angle = 0.0f;

    _CameraController(camera, delta_time);
    RotateLightAroundTarget(&lights[1], objects[0], light_radius);

    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);

    camera_light->transform.position = camera.position;
    camera_light->direction = camera.forward;

    /*********** RENDERING ***********/

    for (int i = 0; i < objects_count; i++)
    { 
      uint32_t u_shininess;     
      if (using_phong_lighting)
        u_shininess = glGetUniformLocation(shader.id(), "u_material.shininess");
      else
        u_shininess = glGetUniformLocation(shader.id(), "u_shininess");
      if (i == 0)
      {
        crate_texture.Bind(0);
        crate_specular_texture.Bind(1);
        glUniform1i(u_shininess, object_0_shininess);
      }
      else
      {
        floor_texture.Bind(0);
        floor_specular_texture.Bind(1);
        glUniform1i(u_shininess, object_1_shininess);
      }

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, objects[i].position);
      model = glm::scale(model, objects[i].scale);
      model = glm::rotate(model, glm::radians(objects[i].angle), objects[i].rotation);

      glm::mat4 view_model = view * model;

      uint32_t u_projection = glGetUniformLocation(shader.id(), "u_projection");
      uint32_t u_view_model = glGetUniformLocation(shader.id(), "u_view_model");
      glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
      glUniformMatrix4fv(u_view_model, 1, GL_FALSE, &view_model[0][0]);

      uint32_t u_material_diffuse = glGetUniformLocation(shader.id(), "u_material.diffuse");
      uint32_t u_material_specular = glGetUniformLocation(shader.id(), "u_material.specular");
      glUniform1i(u_material_diffuse, 0);
      glUniform1i(u_material_specular, 1);

      for (int i = 0; i < lights.size(); i++)
      {
        std::string name = "u_lights[" + std::to_string(i) + "].";

        uint32_t u_light_type = glGetUniformLocation(shader.id(), std::string(name + "type").c_str());
        uint32_t u_light_position = glGetUniformLocation(shader.id(), std::string(name + "position").c_str());
        uint32_t u_light_direction = glGetUniformLocation(shader.id(), std::string(name + "direction").c_str());
        uint32_t u_light_ambient = glGetUniformLocation(shader.id(), std::string(name + "ambient").c_str());
        uint32_t u_light_diffuse = glGetUniformLocation(shader.id(), std::string(name + "diffuse").c_str());
        uint32_t u_light_specular = glGetUniformLocation(shader.id(), std::string(name + "specular").c_str());
        uint32_t u_light_constant = glGetUniformLocation(shader.id(), std::string(name + "constant").c_str());
        uint32_t u_light_linear = glGetUniformLocation(shader.id(), std::string(name + "linear").c_str());
        uint32_t u_light_quadratic = glGetUniformLocation(shader.id(), std::string(name + "quadratic").c_str());
        uint32_t u_light_spot_start_fade = glGetUniformLocation(shader.id(), std::string(name + "spot_start_fade").c_str());
        uint32_t u_light_spot_cutoff = glGetUniformLocation(shader.id(), std::string(name + "spot_cutoff").c_str());
        uint32_t u_lights_count = glGetUniformLocation(shader.id(), "u_lights_count");

        glm::vec3 position = glm::vec3(view * glm::vec4(lights[i].transform.position, 1.0f));
        
        // tbh I have no idea why I have to transpose and inverse the view matrix for directional light to work
        // thought I would have just needed to multiple it by the view matrix without any changes, also
        // this really should be calculated every single light lmao
        glm::vec3 direction = glm::vec3(glm::transpose(glm::inverse(view)) * glm::vec4(lights[i].direction, 1.0f));

        glUniform1i(u_light_type, (int)lights[i].type);
        glUniform3f(u_light_position, position.x, position.y, position.z);
        glUniform3f(u_light_direction, direction.x, direction.y, direction.z);
        glUniform3f(u_light_ambient, lights[i].ambient.r, lights[i].ambient.g, lights[i].ambient.b);
        glUniform3f(u_light_diffuse, lights[i].diffuse.r, lights[i].diffuse.g, lights[i].diffuse.b);
        glUniform3f(u_light_specular, lights[i].specular.r, lights[i].specular.g, lights[i].specular.b);
        glUniform1f(u_light_constant, lights[i].constant);
        glUniform1f(u_light_linear, lights[i].linear);
        glUniform1i(u_light_quadratic, lights[i].quadratic);
        glUniform1f(u_light_spot_start_fade, glm::cos(glm::radians(lights[i].spot_start_fade)));
        glUniform1f(u_light_spot_cutoff, glm::cos(glm::radians(lights[i].spot_cutoff)));
        glUniform1i(u_lights_count, lights.size());
      }

      glDrawArrays(GL_TRIANGLES, 0, 36);

      crate_texture.Unbind();
    }
    shader.Unbind();
    glBindVertexArray(0);

    RenderLights(lights, &light_shader, &camera, projection, view);

    StartImGui();
    ImGui::Begin("Controller");
    LightControllerImGui(using_phong_lighting, lights, ambient_colour, &light_radius);
    ImGui::End();
    EndImGui();

    // ImGui Window Panel Stuff

    glfwSwapBuffers(_window);

    glfwGetFramebufferSize(_window, &_width, &_height);
    glViewport(0, 0, GetWindowWidth(), GetWindowHeight());

    glfwPollEvents();
  }

  CleanLightVertexData();

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void Application::_InitWindow()
{
  assert(glfwInit() && "failed to initialise glfw for some reason ...");

  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);

#ifdef __APPLE__
  // I have no clue what this is doing, I just know you have to add this if on Mac 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

  _window = glfwCreateWindow(GetWindowWidth(), GetWindowHeight(), "OpenGL Window", nullptr, nullptr);
  assert(_window && "failed to initialise window for some reason ...");

  glfwSetWindowAspectRatio(_window, 16, 9);

  glfwMakeContextCurrent(_window);
  assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && "failed to initialise glad for some reason ...");

  glViewport(0, 0, GetWindowWidth(), GetWindowHeight());
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
}

void Application::_CameraController(Camera& camera, float delta_time)
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