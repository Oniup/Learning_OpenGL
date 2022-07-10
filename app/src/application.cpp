#include <app/application.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

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

void MouseCallback(GLFWwindow* window, double x, double y);

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
  Shader shader("app/assets/shaders/phong.vert", "app/assets/shaders/phong.frag");
  Shader light_shader("app/assets/shaders/light.vert", "app/assets/shaders/light.frag");

  Texture crate_texture("app/assets/images/crate.png", false);
  Texture crate_specular_texture("app/assets/images/crate_specular.png", false);

  Texture floor_texture("app/assets/images/floor.png", false);
  Texture floor_specular_texture("app/assets/images/floor_specular.png", false);

  Transform cube_transform = {};
  cube_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
  cube_transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
  cube_transform.rotation = glm::vec3(0.0f, 0.0f, 1.0f);
  Mesh* cube_mesh = _GenerateCubeMesh({ &crate_texture, &crate_specular_texture }, 1);

  GenerateLightVertexData();

  camera.position = glm::vec3(0.0f, 0.0f, 20.0f);
  camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
  camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);

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
      glfwSetCursorPosCallback(_window, MouseCallback);
    }

    /************ UPDATING ***********/    

    float time = (float)glfwGetTime();
    static float last_time = 0.0f;
    float delta_time = time - last_time;
    last_time = time;


    _CameraController(camera, delta_time);
    RotateLightAroundTarget(&lights[1], cube_transform, light_radius);

    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);

    camera_light->transform.position = camera.position;
    camera_light->direction = camera.forward;

    /*********** RENDERING ***********/

    cube_mesh->Render(&shader, view, projection, lights, &cube_transform);

    RenderLights(lights, &light_shader, &camera, projection, view);

    StartImGui();
    ImGui::Begin("Controller");
    LightControllerImGui(true, lights, ambient_colour, &light_radius);
    ImGui::End();
    EndImGui();

    // ImGui Window Panel Stuff

    glfwSwapBuffers(_window);

    glfwGetFramebufferSize(_window, &_width, &_height);
    glViewport(0, 0, GetWindowWidth(), GetWindowHeight());
    projection = glm::perspective(
      glm::radians(45.0f), (float)GetWindowWidth() / (float)GetWindowHeight(), 
      0.1f, 100.0f
    );

    glfwPollEvents();
  }

  delete cube_mesh;

  CleanLightVertexData();
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

Mesh* Application::_GenerateCubeMesh(const std::vector<Texture*>& textures, int diffuse_count)
{
  std::vector<MeshVertex> vertices = {
  // vertex position          uvs             normals
    {{-1.0f, -1.0f,  1.0f},   {0.0f,  0.0f},  { 0.0,  0.0,  1.0}},
    {{ 1.0f, -1.0f,  1.0f},   {1.0f,  0.0f},  { 0.0,  0.0,  1.0}},
    {{ 1.0f,  1.0f,  1.0f},   {1.0f,  1.0f},  { 0.0,  0.0,  1.0}},
    {{-1.0f,  1.0f,  1.0f},   {0.0f,  1.0f},  { 0.0,  0.0,  1.0}},

    {{-1.0f, -1.0f, -1.0f},   {0.0f,  0.0f},  { 0.0,  0.0, -1.0}},
    {{-1.0f,  1.0f, -1.0f},   {1.0f,  0.0f},  { 0.0,  0.0, -1.0}},
    {{ 1.0f,  1.0f, -1.0f},   {1.0f,  1.0f},  { 0.0,  0.0, -1.0}},
    {{ 1.0f, -1.0f, -1.0f},   {0.0f,  1.0f},  { 0.0,  0.0, -1.0}},

    {{-1.0f,  1.0f, -1.0f},   {0.0f,  0.0f},  { 0.0,  1.0,  0.0}},
    {{-1.0f,  1.0f,  1.0f},   {1.0f,  0.0f},  { 0.0,  1.0,  0.0}},
    {{ 1.0f,  1.0f,  1.0f},   {1.0f,  1.0f},  { 0.0,  1.0,  0.0}},
    {{ 1.0f,  1.0f, -1.0f},   {0.0f,  1.0f},  { 0.0,  1.0,  0.0}},

    {{-1.0f, -1.0f, -1.0f},   {0.0f,  0.0f},  { 0.0, -1.0,  0.0}},
    {{ 1.0f, -1.0f, -1.0f},   {1.0f,  0.0f},  { 0.0, -1.0,  0.0}},
    {{ 1.0f, -1.0f,  1.0f},   {1.0f,  1.0f},  { 0.0, -1.0,  0.0}},
    {{-1.0f, -1.0f,  1.0f},   {0.0f,  1.0f},  { 0.0, -1.0,  0.0}},

    {{ 1.0f, -1.0f, -1.0f},   {0.0f,  0.0f},  { 1.0,  0.0,  0.0}},
    {{ 1.0f,  1.0f, -1.0f},   {1.0f,  0.0f},  { 1.0,  0.0,  0.0}},
    {{ 1.0f,  1.0f,  1.0f},   {1.0f,  1.0f},  { 1.0,  0.0,  0.0}},
    {{ 1.0f, -1.0f,  1.0f},   {0.0f,  1.0f},  { 1.0,  0.0,  0.0}},

    {{-1.0f, -1.0f, -1.0f},   {0.0f,  0.0f},  {-1.0,  0.0,  0.0}},
    {{-1.0f, -1.0f,  1.0f},   {1.0f,  0.0f},  {-1.0,  0.0,  0.0}},
    {{-1.0f,  1.0f,  1.0f},   {1.0f,  1.0f},  {-1.0,  0.0,  0.0}},
    {{-1.0f,  1.0f, -1.0f},   {0.0f,  1.0f},  {-1.0,  0.0,  0.0}}
  };

  std::vector<uint32_t> indices = {
    0,  1,  2,   0,  2,  3,    // front
    4,  5,  6,   4,  6,  7,    // back
    8,  9,  10,  8,  10, 11,   // top
    12, 13, 14,  12, 14, 15,   // bottom
    16, 17, 18,  16, 18, 19,   // right
    20, 21, 22,  20, 22, 23,   // left
  };

  Material material = {};
  for (int i = 0; i < textures.size(); i++)
  {
    if (i >= diffuse_count)
      material.specular.push_back(textures[i]);
    else
      material.diffuse.push_back(textures[i]);
  }

  Mesh* mesh = new Mesh(vertices, indices, material);
  return mesh;
}


void MouseCallback(GLFWwindow* window, double x, double y)
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