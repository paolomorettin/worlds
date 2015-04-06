// PHONG-BLINN LIGHTNING MODEL
// with diffuse and normal map

// VERTEX SHADER

#define MAXLIGHTS 8

uniform int gry_numberOfLights;

varying vec3 v; //view vectors in tangent space
varying vec3 lightvec[MAXLIGHTS]; //light vectors in tangent space
varying vec2 texCoord;

attribute vec3 gry_tangent;
attribute vec3 gry_bitangent;

void main(void)
{
	gl_Position = ftransform(); 
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec3 n = normalize (gl_NormalMatrix * gl_Normal);
	vec3 t = normalize (gl_NormalMatrix * gry_tangent);
	vec3 b = normalize (gl_NormalMatrix * gry_bitangent);
   
	vec3 view = vec3(gl_ModelViewMatrix * gl_Vertex);   
   
	 for (int i=0; i < MAXLIGHTS; i++){  
      
      	if(i < gry_numberOfLights){  
	    	vec3 light;
	      
	    	if (gl_LightSource[i].position.w == 0.0) light = - gl_LightSource[i].position.xyz; 
	    	else light = gl_LightSource[i].position.xyz - view;
	      
	      	lightvec[i].x = dot(light, t);
	      	lightvec[i].y = dot(light, b);
	      	lightvec[i].z = dot(light, n);
      	}
   	} 
   	
   	view = -view;
   	
   	v.x = dot(view, t);
   	v.y = dot(view, b);
   	v.z = dot(view, n);
   
}