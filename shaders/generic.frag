#version 400
precision highp float;

in vec4 v_Color;
out vec4 color;

void main() {
    color = v_Color;
}