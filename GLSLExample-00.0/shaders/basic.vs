#version 330
// point d'entrée du GPU (cf pipeline.pdf)

layout (location = 0) in vec2 vsiPosition;
layout (location = 1) in vec2 vsiTexCoord;

// on rajoute un vec3, qui permet d'affiche des pixels RGB (vec2 se limite aux ixels noirs et blancs)
layout (location = 2) in vec3 vsiColor;
out vec2 vsoTexCoord;
out vec3 vsoColor;

void main(void) {
  gl_Position = vec4(vsiPosition, 0.0, 1.0);
  vsoTexCoord = vsiTexCoord;
  vsoColor = vsiColor;
}
