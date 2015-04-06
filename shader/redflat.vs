// RED FLAT MODEL

// VERTEX SHADER


void main(void)
{   
   gl_Position=gl_ModelViewProjectionMatrix * gl_Vertex;
}