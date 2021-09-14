#version 330 core
in vec2 TexCoords;
 
uniform sampler2D texture_diffuse1;
 
void main()
{
    gl_FragColor = texture2D(texture_diffuse1,TexCoords);
}