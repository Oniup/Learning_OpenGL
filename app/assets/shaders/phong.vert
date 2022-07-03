#version 450 core

layout (location = 0) in vec3 a_vertex_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normals;

uniform mat4 u_view_model;
uniform mat4 u_projection;

out vec3 frag_position;
out vec2 uv;
out vec3 normal;

void main()
{
  frag_position = vec3(u_view_model * vec4(a_vertex_pos.xyz, 1.0));
  normal = normalize(mat3(transpose(inverse(u_view_model))) * a_normals);
  uv = a_uv;
  
  gl_Position = u_projection * vec4(frag_position.xyz, 1.0);
}
