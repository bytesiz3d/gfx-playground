#version 400
// Generic attributes
layout(location = 0) in vec2 position;

out vec4 v_Color;

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
    v_Color = vec4(1.0f);
}
