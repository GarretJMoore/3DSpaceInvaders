#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    //gl_Position = projection * view * model * vec4(position, 1.0);
    //normal = mat3(transpose(inverse(model))) * aNormal;
    //pos = vec3(model * vec4(position, 1.0));
    //gl_Position = projection * view * model * vec4(position, 1.0);
	gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
