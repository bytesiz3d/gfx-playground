#version 400
// Vertex attributes
layout(location=0) in vec2 position;
layout(location=1) in vec3 color;

out vec4 vertexColor; // Since vertex shaders do not draw, we need to pass the color data to the fragment shader

void main(){
    gl_Position = vec4(position, 0.0f, 1.0f);
    vertexColor = vec4(color, 1.0f); // Pass the color to the fragment shader
}
