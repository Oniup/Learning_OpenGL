#version 450 core

out vec4 frag_colour;
uniform vec4 light_colour;;

void main()
{
  frag_colour = light_colour;
}