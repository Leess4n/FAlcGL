#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D faceTexture;

uniform float faceness;

void main()
{
    FragColor = mix(texture(ourTexture, TexCoord), texture(faceTexture, TexCoord), faceness);
}