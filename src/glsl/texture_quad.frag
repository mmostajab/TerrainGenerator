#version 430 core

layout(binding = 0) uniform sampler2D color_texture;

layout(location = 0) out vec4 out_color;

in vec2 texcoord;

void main(void)
{
    out_color = texture(color_texture, texcoord);
}