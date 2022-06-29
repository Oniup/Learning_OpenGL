#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <app/transform.hpp>
#include <app/graphics_types.hpp>
#include <app/camera.hpp>
#include <vector>

enum LightType
{
  LIGHT_TYPE_POINT = 0,
  LIGHT_TYPE_DIRECTIONAL,
  LIGHT_TYPE_SPOT_LIGHT
};

struct Light
{
  LightType type;
  Transform transform;
  glm::vec4 colour;
  glm::vec3 direction;
};

void generate_light_vertex_data();
void clean_light_vertex_data();
void rotate_light_around_target(Light* light, const Transform& target, float radius);
void render_lights(const std::vector<Light>& lights, Shader* shader, const Camera* camera, const glm::mat4& projection, const glm::mat4& view);

#endif // __LIGHT_HPP__