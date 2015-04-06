
attribute vec3 tangent;
attribute vec3 bitangent;


void main(void)
{
	gl_Position = ftransform(); 

	gl_FrontColor.rgb = (normalize(bitangent*tangent));
	
}