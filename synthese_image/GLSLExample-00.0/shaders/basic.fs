#version 330
uniform sampler2D myTexture;
in vec2 vsoTexCoord;
flat in vec3 vsoColor;
out vec4 fragColor;

void main(void) {
  //fragColor = texture(myTexture, vsoTexCoord.st);
  //fragColor = vec4(vsoColor, 1);
  fragColor = vec4(vsoColor, 1)+ texture(myTexture, vsoTexCoord.st);
}
