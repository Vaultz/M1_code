#version 330
// point d'entrée du GPU
layout (location = 0) in vec2 vsiPosition;
layout (location = 1) in vec2 vsiTexCoord;
layout (location = 2) in vec3 vsiColor;
out vec2 vsoTexCoord;
flat out vec3 vsoColor;

void main(void) {
  gl_Position = vec4(vsiPosition, 0.0, 1.0);
  vsoTexCoord = vsiTexCoord;
  vsoColor = vsiColor;
}
