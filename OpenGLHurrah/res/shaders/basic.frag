#version 330 core

uniform float time;
in vec3 AColor;
out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0f,1.0f,1.0f, 1.0f);
}