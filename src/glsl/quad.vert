#version 430 core

layout(std140, binding = 0) uniform TransformBlock {
    mat4 proj_mat;
    mat4 view_mat;
    mat4 world_mat;
};

layout(std140, binding = 1) uniform LightBlock {
    vec4 dir;
    vec4 pos;
} ;

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 _texcoord;

out vec2 texcoord;

void main(void)
{
    gl_Position = proj_mat * view_mat * world_mat * position;
	texcoord = _texcoord;
}