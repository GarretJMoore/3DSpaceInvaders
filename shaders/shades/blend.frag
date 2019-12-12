#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform float exposure;

void main() {
	const float gamma = 2.2;
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//FragColor = vec4(texture(scene, TexCoords).rgb, 1.0);
	//FragColor = vec4(texture(bloomBlur, TexCoords).rgb, 1.0);
	
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	hdrColor += bloomColor;
	
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	
	result = pow(result, vec3(1.0 / gamma));
	FragColor = vec4(result, 1.0);
}
