#version 450 core

layout (location = 0) in vec3 a_vert_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main()
{
  gl_Position = projection * view * transform * vec4(a_vert_pos.xyz, 1.0);
}