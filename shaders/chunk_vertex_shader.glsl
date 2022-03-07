#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 instancePosition;
layout (location = 4) in int textureID;

out vec2 textureCoord;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

uniform int textureAtlasRows;

uniform sampler2D aTexture;
uniform int face;

void main() {
    gl_Position = projection * view * transform * vec4(aPos + instancePosition, 1.0);
    //gl_Position = projection * view * vec4(aPos.x + gl_InstanceID, aPos.y, aPos.z, 1.0f);
    textureCoord = ((aTextureCoord + vec2(textureID, 0.0f)) / textureAtlasRows);// + vec2(textureID - (textureID % textureAtlasRows), textureID % textureAtlasRows);
}