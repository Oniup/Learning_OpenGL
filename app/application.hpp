#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <glm/glm.hpp>

typedef struct GLFWwindow GLFWwindow;

struct Transform
{
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  float angle;
};

struct Camera
{
  glm::vec3 position;
  glm::vec3 up;
  glm::vec3 forward;
};

class Application
{
public:
  inline const int window_width() const { return 1280; }
  inline const int window_height() const { return 720; }

public:
  Application();
  ~Application();

public:
  void run();

private:
  void _init_window();
  void _camera_controller(Camera& camera, float delta_time);

private:
  GLFWwindow* _window;
};

#endif // __APPLICATION_HPP__