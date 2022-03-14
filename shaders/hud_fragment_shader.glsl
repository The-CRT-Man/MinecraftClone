#version 330 core

out vec4 fragColour;

in vec2 textureCoord;

uniform sampler2D aTexture;

void main() {
	vec4 textureColour = texture(aTexture, textureCoord);
	fragColour = textureColour;
}