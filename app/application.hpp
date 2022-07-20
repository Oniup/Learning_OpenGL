#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "app/camera.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>

typedef struct GLFWwindow GLFWwindow;
class Mesh;
class Texture;

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
    Mesh* _GenerateCubeMesh(const std::vector<Texture*>& textures, int diffuse_count);

private:
    GLFWwindow* _window;    
    int _width;
    int _height;
};

#endif // __APPLICATION_HPP__