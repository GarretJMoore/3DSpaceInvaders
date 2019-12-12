#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

//in vec3 fragPos;

uniform mat4 model;

uniform int normCol;

uniform vec3 lightCol;
uniform vec3 lightPos;
uniform int cull;

in vec3 normal;

in vec3 pos;

uniform vec3 color;

uniform vec3 viewPos;

uniform int mat;

uniform samplerCube skybox;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
  vec3 I = normalize(pos - viewPos);
  vec3 R = reflect(I, normalize(normal));
  fragColor = vec4(texture(skybox, R).rgb, 1.0);
  
  
  //vec3 norm = normalize(normal);
  //fragColor = vec4( (norm.x+1)/2, (norm.y+1)/2, (norm.z+1)/2, 1.0f );

}
