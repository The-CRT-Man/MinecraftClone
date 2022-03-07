#version 330 core
out vec4 fragColour;

in vec2 textureCoord;

uniform sampler2D aTexture;

void main() {
    fragColour = texture(aTexture, textureCoord);
    //fragColour = vec4(textureCoord.x, textureCoord.y, 0.0f, 1.0f);
}