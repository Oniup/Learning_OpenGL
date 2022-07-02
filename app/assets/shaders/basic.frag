#version 450 core

#define LIGHT_MAX_COUNT 30

struct Light
{
  vec3 position;
  vec3 colour;
};

out vec4 frag_colour;

in vec3 frag_position;
in vec2 uv;
in vec3 normal;

uniform Light u_lights[LIGHT_MAX_COUNT];
uniform int u_lights_count;
uniform vec4 u_ambient_colour;
uniform sampler2D u_texture;

vec3 calc_diffuse(Light light);

void main()
{
  // getting base colour
  vec4 base_colour = texture(u_texture, uv);
  if (u_lights_count >= LIGHT_MAX_COUNT)
  {
    frag_colour = base_colour;
    return;
  }

  // calc lighting
  vec3 light_colour = u_ambient_colour.rgb * u_ambient_colour.a;
  for (int i = 0; i < u_lights_count; i++)
  {
    vec3 diffuse = calc_diffuse(u_lights[i]);
    light_colour += diffuse;
  }

  // final result
  frag_colour = vec4(light_colour.rgb, 1.0) * base_colour;
}

vec3 calc_diffuse(Light light)
{
  vec3 light_direction = normalize(light.position - frag_position);
  float diffuse = max(dot(normal, light_direction), 0.0);
  vec3 result = diffuse * light.colour;
  return result;
}
