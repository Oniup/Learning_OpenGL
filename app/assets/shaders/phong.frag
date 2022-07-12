#version 450 core

#define LIGHT_MAX_COUNT       30
#define LIGHT_TYPE_POINT      0
#define LIGHT_TYPE_DIRECTONAL 1
#define LIGHT_TYPE_SPOT       2

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  int shininess;
};

struct Light
{
  int type;
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float spot_start_fade;
  float spot_end_fade;
};

out vec4 frag_colour;

in vec2 uv;
in vec3 normal;
in vec3 frag_position;

uniform Material u_material;
uniform Light u_lights[LIGHT_MAX_COUNT];
uniform int u_lights_count;

vec3 CalcShading();
vec3 CalcDiffuse(Light light, vec3 light_direction, vec3 diffuse_base);
vec3 GetLightDirection(Light light);

void main()
{
  vec3 shading = CalcShading();
  frag_colour = vec4(shading.rgb, 1.0);
}

vec3 CalcShading()
{
  vec4 diffuse_base = texture(u_material.diffuse, uv);
  vec4 specular_base = texture(u_material.specular, uv);
  diffuse_base.rgb *= diffuse_base.a;
  specular_base.rgb *= specular_base.a;

  vec3 shading = vec3(0.0f);
  for (int i = 0; i < u_lights_count; i++)
  {
    Light light = u_lights[i];
    vec3 light_direction = GetLightDirection(light);

    vec3 diffuse = CalcDiffuse(light, light_direction, diffuse_base.rgb);

    shading += diffuse;
  }

  return shading;
}

vec3 CalcDiffuse(Light light, vec3 light_direction, vec3 diffuse_base)
{
  float diffuse = max(dot(light_direction, normal), 0.0);
  vec3 result = light.diffuse * diffuse_base * diffuse;
  return result;
}

vec3 GetLightDirection(Light light)
{
  vec3 light_direction = vec3(0.0);

  switch (light.type)
  {
    case LIGHT_TYPE_POINT:
      light_direction = light.position - frag_position;
      break;
    case LIGHT_TYPE_DIRECTONAL:
      light_direction = light.direction;
      break;
    case LIGHT_TYPE_SPOT:
      light_direction = light.position - frag_position;
      break;
  }

  return normalize(light_direction);
}
