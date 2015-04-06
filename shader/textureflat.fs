// TEXTURE FLAT MODEL

// FRAGMENT SHADER

uniform sampler2D tex;
uniform sampler2D tex2;


varying vec2 texCoord;

void main(void)
{  
	if (texture2D(tex2, texCoord).r > 0.5){ 
		gl_FragColor=vec4(texture2D(tex,texCoord).rgb,1.0);
	} else {
		gl_FragColor=vec4(texture2D(tex,texCoord).bgr,1.0);
	}
}