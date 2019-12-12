#version 330 core

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 


struct PointLight {
    vec3 position;
    
    float constant;
    float linear; // We are using this one
    float quadratic;
	
    vec3 color;

};


// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 Normal;
in vec3 FragPos;


// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;


uniform vec3 viewPos;
uniform PointLight pointLights; // If multiple lights, change this to # Point Lights
uniform vec3 color; 
uniform Material material;
uniform int isLight;

uniform int shadeType; // Are we using Phong Shading or vertex normal based


// Define functions
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcNormalLight();

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcPointLight(pointLights, norm, FragPos, viewDir);

	if(shadeType == 1) {
		result = CalcNormalLight();
	}

	else if(isLight == 1) {
		result = color;
	}

    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(CalcNormalLight(), 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	    
	vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float p = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), p);

	// attenuation
	float distance = length(light.position - fragPos);
	vec3 attenuation = light.color / (light.linear * distance);

	vec3 ambient = material.ambient; 
	vec3 diffuse = material.diffuse * diff;  
	vec3 specular = spec * material.specular; 
	

	vec3 finalC = ambient + diffuse + specular;
	finalC = finalC * attenuation;
	return finalC;
}

vec3 CalcNormalLight() {
	
	vec3 norm = normalize(Normal);

	float x = (norm.x + 1) / 2;
	float y = (norm.y + 1) / 2;
	float z = (norm.z + 1) / 2;

	return vec3(x,y,z);
}