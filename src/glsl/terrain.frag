#version 430 core

out vec4 out_color;

layout(std140, binding = 1) uniform LightBlock {
    vec4 dir;
    vec4 pos;
} ;

layout( location = 0 ) uniform vec3 color = vec3(1.0f, 0.0f, 0.0f);

layout (binding = 0)	uniform sampler2D side_texture;
layout (binding = 1)	uniform sampler2D above_texture;


in VS_OUT {
	vec3 normal;
	vec3 texcoord;
} fs_in;

void main() {

	// Check if the point is on top / side
	if(fs_in.texcoord.y > 0.0f && fs_in.texcoord.y < 1.0f) 
		out_color =  texture(side_texture, fs_in.texcoord.xy);
	else  
		out_color =  texture(above_texture, fs_in.texcoord.xz);

	out_color *= max(dot(dir.xyz, fs_in.normal), 0.2f);
	out_color += vec4(0.15f, 0.15f, 0.15f, 0.0f);
}