// PHONG-BLINN LIGHTNING MODEL

// FRAGMENT SHADER

#define MAXLIGHTS 8

uniform int gry_numberOfLights;

//material parameters
uniform vec3 kDiff;      //diffuse
uniform vec3 kSpec;      //specular
uniform vec3 kAmb;       //ambient
uniform vec3 kEmi;       //emissive
uniform float m;         //smoothness exponent

varying vec3 n, v;

float Cos(vec3 a, vec3 b){
   return max(0, dot(normalize(a), normalize(b)));
}

void main(void)
{
   vec3 color = kEmi;
  
   for (int i=0; i < MAXLIGHTS; i++){  
      
      if(i < gry_numberOfLights){
	      vec3 L;
	      vec3 V;
	      vec3 H;
	      
	      if (gl_LightSource[i].position.w == 0.0) L = - gl_LightSource[i].position.xyz; 
	      else L = gl_LightSource[i].position.xyz - v;
	      
	      V = -v;
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