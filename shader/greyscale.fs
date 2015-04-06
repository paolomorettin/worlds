uniform sampler2D gry_texture;

void main()
{
	vec3 original_color = texture2D(gry_texture, gl_TexCoord[0].st ).rgb; 
	vec3 luminance_converter = vec3(0.2126, 0.7152, 0.0722);

	float luminance = dot(original_color, luminance_converter); 

	gl_FragColor = vec4(vec3(luminance),1.0);
}
