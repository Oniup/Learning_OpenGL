#version 450 core

#define LIGHT_TYPE_POINT        0
#define LIGHT_TYPE_DIRECTIONAL  1
#define LIGHT_TYPE_SPOT         2

#define LIGHT_MAX_COUNT 30

struct Light
{
  int type;
  vec3 position;  
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  int shininess;
};

out vec4 frag_colour;

in vec3 frag_position;
in vec2 uv;
in vec3 normal;

uniform Material u_material;
uniform Light u_lights[LIGHT_MAX_COUNT];
uniform int u_lights_count;

vec4 calc_lighting();
vec3 calc_diffuse(Light light, vec3 light_direction);
vec3 calc_specular(Light light, vec3 light_direction);
vec3 get_light_direction(Light light, vec3 frag_position);

void main()
{
  vec4 light_colour = calc_lighting();
  frag_colour = light_colour;
}

vec4 calc_lighting()
{
  vec3 light_colour = vec3(0.0);
  vec3 ambient_light = vec3(0.0);
  for (int i = 0; i < u_lights_count; i++)
  {
    vec3 light_direction = get_light_direction(u_lights[i], frag_position);
    vec3 diffuse = calc_diffuse(u_lights[i], light_direction);
    vec3 specular = calc_specular(u_lights[i], light_direction);
    light_colour += diffuse + specular;

    if (u_lights[i].type == LIGHT_TYPE_DIRECTIONAL)
      ambient_light += u_lights[i].ambient;
  }

  return vec4(ambient_light.rgb + light_colour.rgb, 1.0);
}

vec3 calc_diffuse(Light light, vec3 light_direction)
{
  vec3 diffuse_base = texture(u_material.diffuse, uv).rgb;
  float diffuse = max(dot(light_direction, normal), 0.0);
  return diffuse_base * diffuse * light.diffuse;
}

vec3 calc_specular(Light light, vec3 light_direction)
{
  vec3 specular_base = texture(u_material.specular, uv).rgb;
  vec3 view_direction = normalize(-frag_position);
  vec3 reflection = reflect(-light_direction, normal);
  float specular = pow(max(dot(view_direction, reflection), 0.0), u_material.shininess);
  
  return specular_base * specular * light.specular;
}

vec3 get_light_direction(Light light, vec3 frag_position)
{
  vec3 direction;

  switch (light.type)
  {
    case LIGHT_TYPE_POINT:
      direction = light.position - frag_position;
      break;
    case LIGHT_TYPE_DIRECTIONAL:
      direction = light.direction;
      break;
    case LIGHT_TYPE_SPOT:
      direction = vec3(0.0);
      break;
  }

  direction = normalize(direction);
  return direction;
}
