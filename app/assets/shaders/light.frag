#version 450 core

out vec4 frag_colour;

uniform vec3 u_light_colour;

void main()
{
  frag_colour = vec4(u_light_colour.rgb, 1.0);
}