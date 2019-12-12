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

in vec3 normal;

in vec3 fragPos;

uniform vec3 color;

uniform vec3 viewPos;

uniform int mat;

uniform vec3 ambV;
uniform vec3 diffV;
uniform vec3 specV;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;



void main()
{
	if (normCol != 1 ) {
		float ambientStrength = 0.1;
		vec3 ambient = ambientStrength * lightCol;
	
		vec3 lightDir = normalize(lightPos - fragPos);
		float distance = length(lightPos - fragPos);
		float attenuation = 1.0 / (1 + .01 * distance);
		
		float diff = max(dot(normal, lightDir), 0.0f);
		vec3 diffuse = diff * lightCol;
		
		float specularStrength = 0.5;
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = specularStrength * spec * lightCol;
		
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
		
		vec3 result;
		if (mat == 0) {
			result = (ambient + diffuse) * color;
		}
		else if (mat == 1) {
			result = (ambient + specular) * color;
		}
		else if (mat == 2) {
			result = (ambient + diffuse + specular) * color;
		}
		else if (mat == 3) {
			float ambientStrength = 0.8;
			ambient = ambientStrength * lightCol;
			result = (ambient) * color;
		}
		
		fragColor = vec4(result, 1.0);
		//fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else {
		vec3 norm = normalize(normal);
		fragColor = vec4( (norm.x+1)/2, (norm.y+1)/2, (norm.z+1)/2, 1.0f );
	}
    //float ambientStrength = 0.1;
    //vec3 ambient = ambientStrength 
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    //fragColor = vec4(color, sampleExtraOutput);
    //fragColor = vec4(0.5f, 0.5, 0.5, 0.5f);

    //vec3 norm = vec3(model * vec4(normal, 1.0));
	//vec3 norm = normal;
	//vec3 normal2 = normal;
	//float distance = normal2.x *normal2.x + normal2.y*normal2.y + normal2.z*normal2.z;
	//distance = sqrt(distance);
	//normal2.x = normal2.x/distance;
	//normal2.y = normal2.y/distance;
	//normal2.z = normal2.z/distance;
    
    //cout << normal.x << " " << normal.y << " " << normal.z " ";

    //fragColor = vec4(normal.x, normal.y, normal.z, 1.0f);
	//fragColor = vec4((norm.x+1)/2, (norm.y+1)/2, (norm.z+1)/2, 1.0f);

    //vec3 norm = normalize(normal);

}
