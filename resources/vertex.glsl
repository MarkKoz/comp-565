#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour_initial;
out vec3 colour;

void main()
{
   gl_Position = vec4(position, 1.0);
   colour = colour_initial;
}
