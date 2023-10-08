#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in float a_SlotIndex;

out vec2 v_TexCoord;
out float v_SlotIndex;

uniform mat4 u_ViewProjection;

void main() 
{
	v_TexCoord = a_TexCoord;
	v_SlotIndex = a_SlotIndex;
	gl_Position = u_ViewProjection * vec4(a_Pos, 1.0f);	
}