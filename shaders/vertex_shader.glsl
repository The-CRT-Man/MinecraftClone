#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 textureCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    textureCoord = aTextureCoord;
    gl_Position = projection * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}