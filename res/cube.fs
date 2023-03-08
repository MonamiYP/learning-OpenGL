#version 330 core

out vec4 fragment_colour;

uniform vec4 u_cubeColour;
uniform vec4 u_lightColour;

void main() {
    fragment_colour = u_cubeColour * u_lightColour;
}