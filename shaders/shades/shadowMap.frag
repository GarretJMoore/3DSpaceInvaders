#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;

//uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, float dotLightNormal) {
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0, 1] range
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0) {
		projCoords.z = 1.0;
	}
	// get closest depth value form light's persepctive 
	// (using [0,1] range fragPosLight as coords
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	
	float bias = max( 0.005 * (1.0 - dotLightNormal), 0.005 );
	//float shadow = (closestDepth + bias) >  ? 1.0 : 0.0;
	
	// PCF (percentage-closer filter)
	float shadow = 0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	//float shadow = currentDepth > closestDepth ? 1.0: 0.0;
	//float shadow = currentDepth > (closestDepth + bias) ? 1.0: 0.0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float depth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth > (depth + bias) ? 1.0 : 0.0;
		}
	}
	
	return shadow;
}

void main() {
	//vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
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
	// calculate the shadow
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, dotLightNormal);
	//vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	vec3 lighting = (1.0 - shadow) * vec3( (normal.x + 1)/2,
		(normal.y + 1)/2, (normal.z + 1)/2);
	
	FragColor = vec4(lighting, 1.0);
}
