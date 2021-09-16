#version 330 core

in vec2 TexCoords;
out vec4 Color;

uniform sampler2D Texture0;

void main() {
    Color = texture(Texture0, TexCoords);
}
