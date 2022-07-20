#version 450 core

layout (location = 0) in vec3 a_vertex_pos;

uniform mat4 u_final_matrix;

void main()
{
    gl_Position = u_final_matrix * vec4(a_vertex_pos.xyz, 1.0);
}
