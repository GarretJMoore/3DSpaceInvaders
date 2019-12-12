#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D screenTexture;
//uniform sampler2D depthTex;

uniform mat4 projection;
uniform mat4 view;

uniform mat4 pastProject;
uniform mat4 pastView;

void main() {
  //FragColor = texture(screenTexture, TexCoords);
  FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  
  /*
  //mat4 viewPro = projection * view;
  mat4 inverseVP = inverse(projection);

  mat4 pastVP = pastProject * pastView;
  
  vec4 tmp1, tmp2;
  //vec2 UV = TexCoords.xy;
  //float zOverW = texture(depthTex, UV).z;

  vec4 depth = texture(depthTex, TexCoords);
  float zOverW = depth.z;
  //float zOverW = depth.z;
  vec4 H = vec4(TexCoords.x * 2 - 1, (1 - TexCoords.y) * 2 - 1,
      zOverW, 1);

  //vec4 D = mul(H, inverseVP);
  vec4 D = H * inverseVP;
  vec4 worldPos = D / D.w;
  
  vec4 currentPos = H;

  //vec4 previousPos = mul(worldPos, pastVP);
  vec4 previousPos = worldPos * pastProject;
  //vec4 previousPos = pastProject * worldPos;
  previousPos /= previousPos.w;

  vec2 velocity = ((currentPos - previousPos) / 10.0f).xy;

  vec4 color = texture(screenTexture, TexCoords);
  vec2 coords = TexCoords;
  mat4 zero;
  //FragColor = vec4(velocity, 0, 1);
  for (int i = 0; i < 4; i++) {
	zero[i] = vec4(0, 0, 0, 0);
  }
  
//  if (viewPro == zero) {
//	  FragColor = vec4(1,0,1,1);
//  }
//  else {
//	  FragColor = vec4(0,1,0,1);
//  }
  
  //FragColor = texture(screenTexture, TexCoords);
  //FragColor = vec4(TexCoords.x*2-1, (1-TexCoords.y)*2-1, depth.z, 1);
  //FragColor = texture(screenTexture, TexCoords);
  coords += velocity;
  for (int i = 0; i < 10; i++, coords += velocity) {
    vec4 currentColor = texture(screenTexture, coords);
    color += currentColor;
  }
  //FragColor = color / 10;
  */
}
