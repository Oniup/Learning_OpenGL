#version 450 core

out vec4 frag_colour;

in vec2 uv;
in vec3 normal;

uniform sampler2D texture_sample;

void main()
{
  frag_colour = texture(texture_sample, uv);
}
