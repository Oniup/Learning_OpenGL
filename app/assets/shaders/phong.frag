#version 450 core

#define MAX_DIFFUSE_TEXTURES  1
#define MAX_SPECULAR_TEXTURES 1

struct Material
{
  sampler2D diffuse[MAX_DIFFUSE_TEXTURES];
  sampler2D specular[MAX_SPECULAR_TEXTURES];
  int diffuse_count;
  int specular_count;
};

out vec4 frag_colour;

in vec2 uv;
in vec3 normal;
in vec3 frag_position;

uniform Material u_material;

void main()
{
  vec4 diffuse_base = vec4(0.0f);
  vec4 specular_base = vec4(0.0f);
  for (int i = 0; i < u_material.diffuse_count; i++)
    diffuse_base += texture(u_material.diffuse[i], uv);
  for (int i = 0; i < u_material.specular_count; i++)
    specular_base += texture(u_material.specular[i], uv);

  frag_colour += diffuse_base + specular_base;
}
