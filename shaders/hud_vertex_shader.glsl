#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 textureCoord;

uniform mat3 transform;

void main() {
	vec3 pos = transform * vec3(aPos, 0.0f);

	gl_Position = vec4(pos, 1.0f);
	textureCoord = aTextureCoord;
}