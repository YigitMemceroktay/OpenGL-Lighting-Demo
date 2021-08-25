#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform float offset;
void main()
{ 
    TexCoords = aTexCoords;
    //TexCoords.x += sin(TexCoords.y * 4*2*3.14159 + offset) / 100;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  