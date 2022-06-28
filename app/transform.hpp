#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>

struct Transform
{
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  float angle;
};

#endif // __TRANSFORM_HPP__
