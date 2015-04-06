// COOK-TORRANCE SHADING ALGORHITM
// with diffuse and specular map

// FRAGMENT SHADER

#define MAXLIGHTS 8
#define PI 3.14
#define MAXEXPONENT 512.0

uniform int gry_numberOfLights;

//material parameters
uniform vec3 kAmb;       //ambient
uniform vec3 kSpec;		//specular
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

varying vec3 n, v;
varying vec2 texCoord;

float m;

float Cos(vec3 a, vec3 b){
   return max(0, dot(normalize(a), normalize(b)));
}

float beckermannTerm(float angle){
	float A = 1.0 / (pow(m,2.0)+pow(angle,4.0)*PI);
	float B = exp( - pow( tan(acos(angle)) , 2.0) / pow(m,2.0));
    
   return A * B; //CONTROLLARE QUA
}

float geometryTerm(float NdotH, float NdotV, float NdotL, float VdotH){
   float G1 = 2.0 * NdotH * NdotV / VdotH;
   float G2 = 2.0 * NdotH * NdotL / VdotH;
   return min(1.0, min(G1,G2));
}

vec3 fresnelTerm(float VdotH){
	return kSpec + (1.0 - kSpec) * pow(1.0 - VdotH, 5.0);
}

void main(void)
{
   
   
   vec3 kDiff = texture2D(diffuseMap, texCoord).rgb;
	m = texture2D(specularMap, texCoord).r + texture2D(specularMap, texCoord).g + texture2D(specularMap, texCoord).b;
	m = (m/3.0) * MAXEXPONENT; 
	
  vec3 color = vec3(0);
  
   for (int i=0; i < MAXLIGHTS; i++){  
      if (i < gry_numberOfLights){
	      vec3 L;
	      vec3 V;
	      vec3 H;
	      
	      if (gl_LightSource[i].position.w == 0.0) L = - gl_LightSource[i].position.xyz; 
	      else L = gl_LightSource[i].position.xyz - v;
	      
	      V = -v;
	      H = V + L;
	      
	      float NdotL = Cos(n, L);
	      float VdotH = Cos(V, H);
	      float NdotH = Cos(n, H);
	      float NdotV = Cos(n, V);
	      
	      vec3 ambient = gl_LightSource[i].ambient.xyz * kAmb;
	      vec3 diffuse = gl_LightSource[i].diffuse.xyz * NdotL * kDiff;
	      vec3 specular = gl_LightSource[i].specular.xyz * NdotL * fresnelTerm(VdotH) * geometryTerm(NdotH,NdotV,NdotL,VdotH)/(NdotL*NdotV);
	      
	      color = color + ambient + diffuse + specular;  
      }
   } 
   
   gl_FragColor = vec4(clamp(color, 0.0, 1.0), 1.0);
}