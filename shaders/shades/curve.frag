#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;

//in vec3 fragPos;

uniform mat4 model;

uniform int normCol;

uniform vec3 lightCol;
uniform vec3 lightPos;
uniform int cull;

in vec3 normal;

in vec3 fragPos;

in vec3 texCoords;

uniform samplerCube tex;

uniform vec3 color;

uniform vec3 viewPos;

uniform int mat;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;



void main()
{
  fragColor = vec4(color, 1.0f);

}
