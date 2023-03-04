#version 330 core

in vec4 v_vertexColor;
in vec2 v_texCoord;

layout(location = 0) out vec4 fragment_colour;

uniform float multiple;
uniform sampler2D u_texture;

void main() {
    //fragment_colour = *vec4(v_vertexColor.xy * multiple, v_vertexColor.zw);
    vec4 texColor = texture(u_texture, v_texCoord);
    fragment_colour = texColor * vec4(v_vertexColor.xy * multiple, v_vertexColor.zw);
}