#version 330 core

uniform float time;
out vec4 fragColor;

void main()
{
	fragColor = vec4(1-time, time, 1-time, 1.0f);
}