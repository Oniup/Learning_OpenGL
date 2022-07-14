#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "app/transform.hpp"
#include "app/graphics_types.hpp"
#include "app/camera.hpp"

#include <vector>

enum LightType
{
  LIGHT_TYPE_POINT = 0,
  LIGHT_TYPE_DIRECTIONAL,
  LIGHT_TYPE_SPOT
};

struct Light
{
  Transform transform;

  LightType type;
  glm::vec3 direction;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float spot_start_fade;
  float spot_end_fade;
};

void GenerateLightVertexData();
void CleanLightVertexData();
void RotateLightAroundTarget(Light* light, const Transform& target, float radius);
void RenderLights(const std::vector<Light>& lights, Shader* shader, const Camera* camera, const glm::mat4& projection, const glm::mat4& view);

#endif // __LIGHT_HPP__