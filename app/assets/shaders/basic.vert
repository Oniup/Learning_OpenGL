#version 450 core

layout (location = 0) in vec3 a_vertex_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

out vec2 uv;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main()
{
  gl_Position = projection * view * transform * vec4(a_vertex_pos.xyz, 1.0);

  uv = a_uv;
  normal = a_normal;
}
