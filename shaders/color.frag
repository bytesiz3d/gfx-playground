#version 400
precision highp float;

in vec4 vertexColor; // An input received from the vertex shader. Since the vertex shader only send 3 colors (one for each vertex), the rasterizer will interpolate the 3 values to get values for the fragments in the middle of the triangle

out vec4 color;

void main(){
    color = vertexColor; // Send our interpolated color
}
