#version 330 core
layout(location = 0) out vec4 fragment_colour;
in vec4 program_color;
uniform float multiple;
void main() {
    fragment_colour = vec4(program_color.xy * multiple, program_color.zw);
}