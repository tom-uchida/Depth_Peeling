#version 120
#include "qualifire.h"

// Output parameters to fragment shader.
VertOut vec3 position;
VertOut vec3 normal;

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewMatrix; // model-view matrix
uniform mat4 ModelViewProjectionMatrix; // model-view projection matrix
uniform mat3 NormalMatrix; // normal matrix

void main()
{
    gl_Position = ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;

    position = ( ModelViewMatrix * gl_Vertex ).xyz;
    normal = NormalMatrix * gl_Normal;
}
