#version 330
uniform sampler2D myTexture;
uniform float width, height;
in  vec2 vsoTexCoord;
out vec4 fragColor;

vec2 pas = vec2(1.0 / float(width - 1), 1.0 / float(height - 1));
const float blurMat[25] = float[]( 1.0 / 273.0,  4.0 / 273.0,  7.0 / 273.0,  4.0 / 273.0, 1.0 / 273.0,
				   4.0 / 273.0, 16.0 / 273.0, 26.0 / 273.0, 16.0 / 273.0, 4.0 / 273.0,
				   7.0 / 273.0, 26.0 / 273.0, 41.0 / 273.0, 26.0 / 273.0, 7.0 / 273.0,
				   4.0 / 273.0, 16.0 / 273.0, 26.0 / 273.0, 16.0 / 273.0, 4.0 / 273.0,
				   1.0 / 273.0,  4.0 / 273.0,  7.0 / 273.0,  4.0 / 273.0, 1.0 / 273.0 );
vec2 offset[25] = vec2[](
			 vec2(-2.0 * pas.x , -2.0 * pas.y), vec2(-pas.x, -2.0 * pas.y), vec2(0.0 , -2.0 * pas.y), vec2(pas.x , -2.0 * pas.y), vec2(2.0 * pas.x , -2.0 * pas.y), 
			 vec2(-2.0 * pas.x , -pas.y), vec2(-pas.x, -pas.y), vec2(0.0 , -pas.y), vec2(pas.x , -pas.y), vec2(2.0 * pas.x , -pas.y), 
			 vec2(-2.0 * pas.x , 0.0), vec2(-pas.x, 0.0), vec2(0.0 , 0.0), vec2(pas.x , 0.0), vec2(2.0 * pas.x , 0.0), 
			 vec2(-2.0 * pas.x , pas.y), vec2(-pas.x, pas.y), vec2(0.0 , pas.y), vec2(pas.x , pas.y), vec2(2.0 * pas.x , pas.y), 
			 vec2(-2.0 * pas.x , 2.0 * pas.y), vec2(-pas.x, 2.0 * pas.y), vec2(0.0 , 2.0 * pas.y), vec2(pas.x , 2.0 * pas.y), vec2(2.0 * pas.x , 2.0 * pas.y)
);

vec3 blur(void) {
  vec3 b = vec3(0.0, 0.0, 0.0);
  for(int i = 0; i < 25; i++)
    b += texture(myTexture, vsoTexCoord.st + offset[i]).rgb * blurMat[i];
  return b;
}

void main(void) {
  //int t = int(vsoTexCoord.t * height);
  //if((t%4) != 0)
  fragColor = vec4(blur(), 1.0);
  //else
  //fragColor = vec4(1.1, 1.0, 1.0, 1.0) * texture(myTexture, vsoTexCoord.st);
}
