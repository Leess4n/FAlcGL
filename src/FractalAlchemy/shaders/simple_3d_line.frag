#version 330 core

in vec3 vertexPosition;

out vec4 fragColor;

void main()
{
    // fragColor = vec4(1.0);
    fragColor = vec4(
        abs(vertexPosition.x) / 20,
        abs(vertexPosition.y) / 20,
        abs(vertexPosition.z) / 20,
        1.0
    );
}