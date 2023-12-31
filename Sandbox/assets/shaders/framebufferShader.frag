#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {

	vec3 col = texture(u_Texture, v_TexCoord).rgb;
    FragColor = vec4(col, 1.0);

}