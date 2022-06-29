#version 450 core

layout (location = 0) in vec3 a_vertex_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

out vec2 uv;
out vec3 normal;
out vec3 frag_position;
out mat4 view_transform;

uniform mat4 u_mvp;
uniform mat4 u_view_transform;

void main()
{
  gl_Position = u_mvp * vec4(a_vertex_pos.xyz, 1.0);

  uv = a_uv;
  normal = normalize(mat3(transpose(inverse(u_view_transform))) * a_normal);
  frag_position = vec3(u_view_transform * vec4(a_vertex_pos.xyz, 1.0));
  view_transform = u_view_transform;
}
