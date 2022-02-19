#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
out vec3 vertex_colour;

uniform float time;

void main()
{
   gl_Position = vec4(position, 1.0);
   vertex_colour = colour * (sin(time) / 2.0 + 0.5);
}
