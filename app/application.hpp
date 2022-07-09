#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <glm/glm.hpp>
#include <app/camera.hpp>

typedef struct GLFWwindow GLFWwindow;

class Application
{
public:
  inline const int GetWindowWidth() const { return _width; }
  inline const int GetWindowHeight() const { return _height; }

  inline GLFWwindow* GetNativeWindow() { return _window; }
  inline const GLFWwindow* GetNativeWindow() const { return _window; }

public:
  Application();
  ~Application();

public:
  void Run();

private:
  void _InitWindow();
  void _CameraController(Camera& camera, float delta_time);

private:
  GLFWwindow* _window;  
  int _width;
  int _height;
};

#endif // __APPLICATION_HPP__