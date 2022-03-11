#version 330 core
out vec4 fragColour;

in vec2 textureCoord;

uniform sampler2D aTexture;
uniform int face;

void main() {
    float lighting;
    if (face == 0)
        lighting = 1.0f;
    else if (face == 1)
        lighting = 0.5f;
    else if (face == 2 || face == 3)
        lighting = 0.6f;
    else if (face == 4 || face == 5)
        lighting = 0.75f;

    fragColour = texture(aTexture, textureCoord) * lighting;
    //if (fragColour.a < 0.1)
        //discard;
    //fragColour = vec4(textureCoord.x, textureCoord.y, 0.0f, 1.0f);
}