// PHONG-BLINN LIGHTNING MODEL

// FRAGMENT SHADER

uniform float red;
uniform vec2 remaining; 
 
void main(void)
{
	gl_FragColor = vec4(red, remaining, 1.0);
}