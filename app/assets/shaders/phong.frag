#version 450 core

#define MAX_MATERIAL_TEXTURE_COUNT  3

#define LIGHT_MAX_COUNT             30
#define LIGHT_TYPE_POINT            0
#define LIGHT_TYPE_DIRECTIONAL      1
#define LIGHT_TYPE_SPOT             2

struct Material
{
  sampler2D diffuse[MAX_MATERIAL_TEXTURE_COUNT];
  sampler2D specular[MAX_MATERIAL_TEXTURE_COUNT];
  int diffuse_count;
  int specular_count;
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
vec3 CalcDiffuseShading(Light light, vec3 light_direction, vec3 diffuse_map);
vec3 CalcSpecularShading(Light light, vec3 light_direction, vec3 specular_map);

vec3 GetDiffuseMap();
vec3 GetSpecularMap();

vec3 GetLightDirection(Light light);

void main()
{
  vec3 shading = CalcShading();

  frag_colour = vec4(shading.rgb, 1.0);
}

vec3 CalcShading()
{
  vec3 diffuse_map = GetDiffuseMap();
  vec3 specular_map = GetSpecularMap();

  vec3 shading = vec3(0.0);
  for (int i = 0; i < u_lights_count; i++)
  {
    Light light = u_lights[i];
    vec3 light_direction = GetLightDirection(light);

    vec3 ambient = light.ambient;
    vec3 diffuse = CalcDiffuseShading(light, light_direction, diffuse_map);
    vec3 specular = CalcSpecularShading(light, light_direction, specular_map);

    if (light.type != LIGHT_TYPE_DIRECTIONAL)
    {
      float light_distance = length(light.position - frag_position);
      float attenuation = 1.0 / (light.constant + light.linear * light_distance + light.quadratic * (light_distance * light_distance));

      ambient  *= attenuation;
      diffuse  *= attenuation;
      specular *= attenuation;

      if (light.type == LIGHT_TYPE_SPOT)
      {
        float theta = dot(light_direction, normalize(-light.direction));
        if (theta < light.spot_end_fade)
        {
          diffuse = vec3(0.0);
          specular = vec3(0.0);
        }
      }
    }

    shading += ambient + diffuse + specular;
  }

  return shading;
}

vec3 CalcDiffuseShading(Light light, vec3 light_direction, vec3 diffuse_map)
{
  float strength = max(dot(light_direction, normal), 0.0);
  vec3 diffuse = light.diffuse * diffuse_map * strength;

  return diffuse;
}

vec3 CalcSpecularShading(Light light, vec3 light_direction, vec3 specular_map)
{
  vec3 view_direction = normalize(-frag_position);
  vec3 reflection = reflect(-light_direction, normal);
  float strength = pow(max(dot(view_direction, reflection), 0.0), u_material.shininess);
  vec3 specular = light.specular * specular_map * strength;

  return specular;
}

vec3 GetDiffuseMap()
{
  vec3 diffuse_map = vec3(0.0);
  for (int i = 0; i < u_material.diffuse_count; i++)
  {
    vec4 map = texture(u_material.diffuse[i], uv);
    diffuse_map += map.rgb * map.a;
  }

  return diffuse_map;
}

vec3 GetSpecularMap()
{
  vec3 specular_map = vec3(0.0);
  for (int i = 0; i < u_material.specular_count; i++)
  {
    vec4 map = texture(u_material.specular[i], uv);
    specular_map += map.rgb * map.a;
  }

  return specular_map;
}

vec3 GetLightDirection(Light light)
{
  vec3 light_direction = vec3(0.0);
  switch (light.type)
  {
    case LIGHT_TYPE_POINT:
      light_direction = light.position - frag_position;
      break;
    case LIGHT_TYPE_DIRECTIONAL:
      light_direction = light.direction;;
      break;
    case LIGHT_TYPE_SPOT:
      light_direction = light.position - frag_position;
      break;
  }

  return normalize(light_direction);
}
