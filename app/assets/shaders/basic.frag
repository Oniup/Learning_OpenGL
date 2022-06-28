#version 450 core

out vec4 frag_colour;

in vec2 uv;
in vec3 normal;

uniform sampler2D texture_sample;

uniform float ambient_strength;
uniform vec3 ambient_colour;

void main()
{
  vec4 base_texture_colour = texture(texture_sample, uv);

  if (ambient_strength < 0.001)
    discard;

  vec3 ambient = ambient_colour * ambient_strength;
  vec4 colour = base_texture_colour * vec4(ambient.rgb, 1.0);

  frag_colour = colour;
}
