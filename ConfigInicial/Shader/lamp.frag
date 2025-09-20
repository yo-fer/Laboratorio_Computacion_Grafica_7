#version 330 core
out vec4 outColor;
  
in vec3 Color;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    outColor = vec4(Color,1.0)*texture(ourTexture, TexCoord);
    
    // Modificaci�n: Si la opacidad es trasnparente, no tomamos en cuenta el fragmento
    if (outColor.a < 0.1)
        discard;
}