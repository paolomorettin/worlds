// COOK-TORRANCE SHADING ALGORHITM

// VERTEX SHADER

varying vec3 n, v;

void main(void)
{
   n = gl_NormalMatrix * gl_Normal;
   v = (gl_ModelViewMatrix * gl_Vertex).xyz;
   
   gl_Position=gl_ModelViewProjectionMatrix * gl_Vertex;
}