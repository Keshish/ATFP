#version 400 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D textureSampler;

void main(){
    FragColor = texture(textureSampler, texCoord);
}
