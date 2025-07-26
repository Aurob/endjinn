#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D uTexture;
uniform vec3 uColor;

void main() {
    vec4 texColor = texture(uTexture, vTexCoord);
    FragColor = vec4(uColor, texColor.a);
}