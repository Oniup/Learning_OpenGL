#version 450 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
};

out vec4 frag_colour;

in vec2 uv;
in vec3 diffuse_lighting;
in vec3 specular_lighting;

uniform Material u_material;
uniform int u_lights_count;
uniform vec4 u_ambient_colour;

void main()
{
  vec3 diffuse = texture(u_material.diffuse, uv).rgb * diffuse_lighting;
  vec3 specular = texture(u_material.specular, uv).rgb * specular_lighting;

  vec3 light_colour = u_ambient_colour.rgb * u_ambient_colour.a;
  light_colour += diffuse + specular;

  frag_colour = vec4(light_colour.rgb, 1.0);
}
