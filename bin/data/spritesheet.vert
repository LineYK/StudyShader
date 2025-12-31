#version 410

layout(location = 0) in vec3 pos;
layout(location = 3) in vec2 uv;

uniform vec2 size;
uniform vec2 offset;

out vec2 fragUV;

void main()
{
	vec3 translation = vec3(0.5, 0.0, 0.0);
	gl_Position = vec4(pos + translation, 1.0);
	fragUV = vec2(uv.x, 1.0 - uv.y) * size + (offset * size);
}