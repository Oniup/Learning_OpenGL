#version 450 core

layout (location = 0) in vec3 a_vertex_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_view_model;
uniform mat4 u_projection;

out vec2 uv;
out vec3 normal;
out vec3 frag_position;

void main()
{
  vec4 position = u_view_model * vec4(a_vertex_pos.xyz, 1.0);
  gl_Position = u_projection * position;

  uv = a_uv;
  normal = normalize(mat3(transpose(inverse(u_view_model))) * a_normal);
  frag_position = position.xyz;
}

