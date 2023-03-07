#version 330 core

layout(location = 0) in vec3 position;
// layout(location = 1) in vec4 transform;
uniform mat4 transform;
uniform mat4 scale;

// vec4 transform = vec4(1.0f, 1.0f, 1.0f, 0.0f);
void main() { gl_Position = transform * scale * vec4(position, 1.0f); }
