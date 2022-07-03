#version 450 core

#define LIGHT_MAX_COUNT 30

struct Light
{
  vec3 position;
  vec3 colour;
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
uniform vec4 u_ambient_colour;

vec4 calc_lighting();
vec3 calc_diffuse(Light light, vec3 light_direction);
vec3 calc_specular(Light light, vec3 light_direction);

void main()
{
  vec4 light_colour = calc_lighting();
  frag_colour = light_colour;
}

vec4 calc_lighting()
{
  vec3 light_colour = u_ambient_colour.rgb * u_ambient_colour.a;
  for (int i = 0; i < u_lights_count; i++)
  {
    vec3 light_direction = normalize(u_lights[i].position - frag_position);
    vec3 diffuse = calc_diffuse(u_lights[i], light_direction);
    vec3 specular = calc_specular(u_lights[i], light_direction);
    light_colour += diffuse + specular;
  }

  return vec4(light_colour.rgb, 1.0);
}

vec3 calc_diffuse(Light light, vec3 light_direction)
{
  vec3 diffuse_base = texture(u_material.diffuse, uv).rgb;
  float diffuse = max(dot(light_direction, normal), 0.0);
  return diffuse_base * diffuse * light.colour;
}

vec3 calc_specular(Light light, vec3 light_direction)
{
  vec3 specular_base = texture(u_material.specular, uv).rgb;
  vec3 view_direction = normalize(-frag_position);
  vec3 reflection = reflect(-light_direction, normal);
  float specular = pow(max(dot(view_direction, reflection), 0.0), u_material.shininess);
  
  return specular_base * specular * light.colour;
}
