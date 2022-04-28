#version 410

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_tex_coord;

void main()
{
	v_tex_coord = a_tex_coord;
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1);
}