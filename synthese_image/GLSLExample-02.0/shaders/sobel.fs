#version 330
uniform sampler2D myTexture;
uniform float width, height;
in  vec2 vsoTexCoord;
out vec4 fragColor;

const vec2 G[9] = vec2[]( vec2(1.0,  1.0), vec2(0.0,  2.0), vec2(-1.0,  1.0), 
			  vec2(2.0,  0.0), vec2(0.0,  0.0), vec2(-2.0,  0.0), 
			  vec2(1.0, -1.0), vec2(0.0, -2.0), vec2(-1.0, -1.0) );
vec2 pas = vec2(1.0 / float(width - 1), 1.0 / float(height - 1));
vec2 offset[9] = vec2[](vec2(-pas.x , -pas.y), vec2( 0.0, -pas.y), vec2( pas.x , -pas.y), 
			vec2(-pas.x, 0.0),       vec2( 0.0, 0.0),      vec2( pas.x, 0.0), 
			vec2(-pas.x,   pas.y), vec2( 0.0, pas.y),  vec2( pas.x ,  pas.y));
vec3 sobel(void) {
  vec2 r = vec2(0.0, 0.0);
  vec2 g = vec2(0.0, 0.0);
  vec2 b = vec2(0.0, 0.0);
  for(int i = 0; i < 9; i++) {
    //g += dot(vec3(0.299, 0.587, 0.114), texture(myTexture, vsoTexCoord.st + offset[i]).rgb) * G[i];
    r += texture(myTexture, vsoTexCoord.st + offset[i]).r * G[i];
    g += texture(myTexture, vsoTexCoord.st + offset[i]).g * G[i];
    b += texture(myTexture, vsoTexCoord.st + offset[i]).b * G[i];
  }
  return /* vec3(1.0) -  */vec3(length(r), length(g), length(b));
}

void main(void) {
  fragColor = /* vec4(texture(myTexture, vsoTexCoord.st).r);// */vec4(sobel(), 1.0);
}
