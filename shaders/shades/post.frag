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
  FragColor = texture(screenTexture, TexCoords);
  //FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
