#version 450 core

out vec4 frag_colour;

in vec2 uv;
in vec3 light_colour;

uniform sampler2D u_texture;

void main()
{
  vec4 base_colour = texture(u_texture, uv);

  frag_colour = vec4(light_colour.rgb, 1.0) * base_colour;
}