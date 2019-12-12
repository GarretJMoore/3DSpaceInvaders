#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	//vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	//vs_out.TexCoords = aTexCoords;

	//gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//TexCoords = aTexCoords;
}
