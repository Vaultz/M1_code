#version 330

layout (location = 0) in vec2 vsiPosition;
layout (location = 2) in vec2 vsiTexCoord;
uniform int inv; 
out vec2 vsoTexCoord;

void main(void) {
  gl_Position = vec4(vsiPosition.x, vsiPosition.y, 0.0, 1.0);
  if(inv != 0)
    vsoTexCoord = vec2(vsiTexCoord.s, 1.0 - vsiTexCoord.t);
  else
    vsoTexCoord = vec2(vsiTexCoord.s, vsiTexCoord.t);
}
