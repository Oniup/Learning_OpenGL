#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>

struct Camera
{
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 forward;
};

#endif // __CAMERA_HPP__