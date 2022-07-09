#version 450 core

#define LIGHT_TYPE_POINT 0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_SPOT 2

#define LIGHT_MAX_COUNT 30

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
  float spot_cutoff;
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
vec3 calc_diffuse(Light light, vec3 light_direction, vec3 diffuse_base);
vec3 calc_specular(Light light, vec3 light_direction, vec3 specular_base);
vec3 get_light_direction(Light light, vec3 frag_position);

void main()
{
  vec4 light_colour = calc_lighting();
  frag_colour = light_colour;
}

vec4 calc_lighting()
{
  vec3 light_colour = vec3(0.0);
  for (int i = 0; i < u_lights_count; i++)
  {
    Light light = u_lights[i];

    vec3 light_direction = get_light_direction(light, frag_position);
    vec3 diffuse_base = texture(u_material.diffuse, uv).rgb;

    vec3 specular_base = texture(u_material.specular, uv).rgb;

    vec3 diffuse = calc_diffuse(light, light_direction, diffuse_base);
    vec3 specular = calc_specular(light, light_direction, specular_base);
    vec3 ambient = diffuse_base * light.ambient;
    
    if (light.type != LIGHT_TYPE_DIRECTIONAL)
    {      
      float light_distance = length(light.position - frag_position);
      float attenuation = 1.0 / (light.constant + light.linear * light_distance + 
    		    light.quadratic * (light_distance * light_distance)); 
      
      diffuse   *= attenuation;
      specular  *= attenuation;
      ambient   *= attenuation;
    }

    if (light.type == LIGHT_TYPE_SPOT)
    {
      float theta = dot(light_direction, normalize(-light.direction));
      float e = light.spot_start_fade - light.spot_cutoff;
      float intensity = clamp((theta - light.spot_cutoff) / e, 0.0, 1.0);

      diffuse *= intensity;
      specular *= intensity;
    }

    light_colour += ambient + diffuse + specular;
  }

  return vec4(light_colour.rgb, 1.0);
}

vec3 calc_diffuse(Light light, vec3 light_direction, vec3 diffuse_base)
{
  float diffuse = max(dot(light_direction, normal), 0.0);
  return diffuse_base * diffuse * light.diffuse;
}

vec3 calc_specular(Light light, vec3 light_direction, vec3 specular_base)
{
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
      direction = light.position - frag_position;
      break;
  }

  direction = normalize(direction);
  return direction;
}
