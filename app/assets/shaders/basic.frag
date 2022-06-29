#version 450 core

#define LIGHT_TYPE_POINT 0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_SPOT_LIGHT 2

#define LIGHT_MAX_COUNT 30
struct Light
{
  int type;
  vec3 colour;
  vec3 position;
  vec3 direction;
};

out vec4 frag_colour;

in vec2 uv;
in vec3 normal;
in vec3 frag_position;
in mat4 view_transform;

uniform sampler2D u_texture_sample;
uniform float u_ambient_strength;
uniform vec3 u_ambient_colour;

uniform Light u_light[LIGHT_MAX_COUNT];
uniform int u_light_count;

vec3 calc_diffuse(Light light);
vec3 calc_specular(Light light);

void main()
{
  // if you get this colour, then there are too many lights in the scene
  if (u_light_count > LIGHT_MAX_COUNT)
  {
    frag_colour = vec4(1.0, 0.0, 0.0, 1.0);
    return;
  }

  vec4 base_texture_colour = texture(u_texture_sample, uv);

  if (u_ambient_strength < 0.01)
    discard;

  // lighting calcs
  vec3 lighting_result = vec3(0.0, 0.0, 0.0);
  for (int i = 0; i < u_light_count; i++)
  {
    vec3 ambient = u_ambient_colour * u_ambient_strength;
    vec3 diffuse = calc_diffuse(u_light[i]);
    vec3 specular = calc_specular(u_light[i]);
    lighting_result += ambient.rgb + diffuse.rgb + specular.rgb;
  }

  // final colour
  vec4 colour = base_texture_colour * vec4(lighting_result.rgb, 1.0);

  // submitting the final colour
  frag_colour = colour;
}

vec3 calc_diffuse(Light light)
{
  vec3 direction = normalize(vec3(view_transform * vec4(light.position, 1.0)) - frag_position);
  float diffuse = max(dot(normal, direction), 0.0);
  return diffuse * light.colour;
}

vec3 calc_specular(Light light)
{
  return vec3(0.0, 0.0, 0.0);
}

