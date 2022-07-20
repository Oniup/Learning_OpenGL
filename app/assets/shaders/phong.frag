#version 450 core

struct Material
{
    // TODO: implement texture samplers here
    int shininess; 
};

out vec4 frag_colour;

in vec2 uv;
in vec3 normal;
in vec3 frag_position;

void main()
{
    frag_colour = vec4(1.0);
}