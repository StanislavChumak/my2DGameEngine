#version 460 core
out vec4 outData;
  
in vec3 color;
in vec2 texturePositeon;

uniform sampler2D textureData;

void main()
{
    outData = texture(textureData, texturePositeon) * vec4(color, 1.0); 
}