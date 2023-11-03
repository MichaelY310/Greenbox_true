#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int FragColor1;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
flat in int v_EntityID;

uniform sampler2D u_Texture[32];

void main() 
{
	FragColor = texture(u_Texture[int(v_TexIndex+0.1)], v_TexCoord) * v_Color;
	FragColor1 = v_EntityID;
	// FragColor = vec4(0.2f, 0.3f, 0.6f, 1.0f);
}