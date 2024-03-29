#version 330 core
//out vec4 FragColor;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	//vec4 FragPosLightSpace;
} fs_in;

//uniform sampler2D diffuseTexture;
//uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	vec3 color = vec3(1.0, 0.0, 0.0);
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0);
	// ambient
	vec3 ambient = 0.3 * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float dotLightNormal = dot(lightDir, normal);
	float diff = max( dotLightNormal, 0.0 );
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow( max( dot(normal, halfwayDir), 0.0 ), 64.0 );
	vec3 specular = spec * lightColor;
	vec3 lighting = (ambient + (diffuse + specular)) * color;
	
	FragColor = vec4(lighting, 1.0);
	//FragColor = vec4(1.0, 0.5, 1.0, 1.0);
	
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	//BrightColor = vec4(brightness, brightness, brightness, 1.0);
	//BrightColor = vec4(lighting, 1.0);
	
	if (brightness > 1.0)
		BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
