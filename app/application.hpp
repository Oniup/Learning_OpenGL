#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <glm/glm.hpp>
#include <app/camera.hpp>

typedef struct GLFWwindow GLFWwindow;


class Application
{
public:
  inline const int window_width() const { return _width; }
  inline const int window_height() const { return _height; }

  inline GLFWwindow* native_window() { return _window; }
  inline const GLFWwindow* native_window() const { return _window; }

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
  int _width;
  int _height;
};

#endif // __APPLICATION_HPP__