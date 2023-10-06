#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main() 
{
	FragColor = texture(u_Texture, v_TexCoord);
	// FragColor = vec4(0.2f, 0.3f, 0.6f, 1.0f);
}