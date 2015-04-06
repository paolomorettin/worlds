// PHONG-BLINN LIGHTNING MODEL
// with diffuse and normal map

// FRAGMENT SHADER

#define MAXLIGHTS 8
#define MAXEXPONENT 512.0

uniform int gry_numberOfLights;

//material parameters
uniform sampler2D diffuseMap;	//diffuse
uniform float m;				//smoothness exponent
uniform vec3 kAmb;       		//ambient
uniform vec3 kSpec;       		//specular
uniform vec3 kEmi;       		//emissive

uniform sampler2D normalMap;	//normal map

varying vec3 v; //view vectors in tangent space
varying vec3 lightvec[MAXLIGHTS]; //light vectors in tangent space
varying vec2 texCoord;

float Cos(vec3 a, vec3 b){
	return max(0, dot(normalize(a), normalize(b)));
}

void main(void)
{
	vec3 kDiff = texture2D(diffuseMap, gl_TexCoord[0].xy).rgb;
	
	
	vec3 n = (2.0 * texture2D(normalMap, gl_TexCoord[0].xy).rgb) - 1.0;
	 
	vec3 color = kEmi;
  
    for (int i=0; i < MAXLIGHTS; i++){  
      
      if(i < gry_numberOfLights){  
	      
	      vec3 L = lightvec[i];
	      vec3 V = v;
	      vec3 H;
	      
	      H = V + L;
	      
	      float NdotL = Cos(n, L);
	      float NdotH = Cos(n, H);
	      
	      vec3 ambient = gl_LightSource[i].ambient.rgb * kAmb;
	      vec3 diffuse = gl_LightSource[i].diffuse.rgb * NdotL * kDiff;
	      vec3 specular = gl_LightSource[i].specular.rgb * NdotL * pow(NdotH,m) * kSpec;
	      
	      color = color + ambient + diffuse + specular;
	   }
   } 
   
   gl_FragColor = vec4(clamp(color, 0.0, 1.0), 1.0);
}
