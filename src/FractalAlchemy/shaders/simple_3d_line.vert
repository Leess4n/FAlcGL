#version 330 core
// Coordinates of the points
layout (location = 0) in float X;
layout (location = 1) in float Y; 
layout (location = 2) in float Z;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(X, Y, Z, 1.0);
}