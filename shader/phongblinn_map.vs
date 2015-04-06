// PHONG-BLINN LIGHTNING MODEL
// with diffuse and specular map

// VERTEX SHADER

varying vec3 n, v;
varying vec2 texCoord;

void main(void)
{
   n = gl_NormalMatrix * gl_Normal;
   v = (gl_ModelViewMatrix * gl_Vertex).xyz;
   
   texCoord = gl_MultiTexCoord0.xy;
   
   gl_Position=gl_ModelViewProjectionMatrix * gl_Vertex;
}