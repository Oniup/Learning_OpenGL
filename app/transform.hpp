#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>

struct Transform
{
    Transform()
        : position(1.0f), scale(1.0f), rotation(0.0f, 1.0f, 0.0f), angle(0.0f)    {}

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    float angle;
};

#endif // __TRANSFORM_HPP__
