// FIXME: This currently doesn't work in this build, will fix the next day
#version 450 core

layout (location = 0) in vec3 a_vertex_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normals;

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

out vec2 uv;
out vec3 diffuse_lighting;
out vec3 specular_lighting;
out vec3 ambient_lighting;

uniform mat4 u_view_model;
uniform mat4 u_projection;

uniform Light u_lights[LIGHT_MAX_COUNT];
uniform int u_lights_count;
uniform int u_shininess;

void calc_lighting(vec3 vertex_position);
vec3 calc_diffuse(Light light, vec3 light_direction, vec3 normal);
vec3 calc_specular(Light light, vec3 vertex_position, vec3 light_direction, vec3 normal);
vec3 get_direction(Light light, vec3 vertex_position);

void main()
{  
  vec3 vertex_position = vec3(u_view_model * vec4(a_vertex_pos.xyz, 1.0));
  gl_Position = u_projection * vec4(vertex_position.xyz, 1.0);

  calc_lighting(vertex_position);
  uv = a_uv;
}

void calc_lighting(vec3 vertex_position)
{
  diffuse_lighting = vec3(0.0);
  specular_lighting = vec3(0.0);
  ambient_lighting = vec3(0.0);

  vec3 normal = normalize(mat3(transpose(inverse(u_view_model))) * a_normals);

  for (int i = 0; i < u_lights_count; i++)
  {
    vec3 light_direction = get_direction(u_lights[i], vertex_position);
    diffuse_lighting += calc_diffuse(u_lights[i], light_direction, normal);
    specular_lighting += calc_specular(u_lights[i], vertex_position, light_direction, normal);

    if (u_lights[i].type == LIGHT_TYPE_DIRECTIONAL)
      ambient_lighting += u_lights[i].ambient;
  }
}

vec3 calc_diffuse(Light light, vec3 light_direction, vec3 normal)
{
  float diffuse = max(dot(light_direction, normal), 0.0);
  return light.diffuse * diffuse;
}

vec3 calc_specular(Light light, vec3 vertex_position, vec3 light_direction, vec3 normal)
{
  vec3 view_directon = normalize(-vertex_position);
  vec3 reflection = reflect(-light_direction, normal);
  float specular = pow(max(dot(view_directon, reflection), 0.0), u_shininess);

  return specular * light.specular;
}

vec3 get_direction(Light light, vec3 vertex_position)
{
  vec3 direction;

  switch (light.type)
  {
    case LIGHT_TYPE_POINT:
      direction = light.position - vertex_position;
      break;
    case LIGHT_TYPE_DIRECTIONAL:
      direction = light.direction;
      break;
    case LIGHT_TYPE_SPOT:
      direction = vec3(0.0);
      break;
  }

  return normalize(direction);
}
