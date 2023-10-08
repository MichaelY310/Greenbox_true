#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in float v_SlotIndex;

uniform sampler2D u_Texture[32];

void main() 
{
	FragColor = texture(u_Texture[int(v_SlotIndex)], v_TexCoord);
	// FragColor = vec4(0.2f, 0.3f, 0.6f, 1.0f);
}