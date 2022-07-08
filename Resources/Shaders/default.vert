#version 330 core

layout (location = 0) in vec3 pos;
out vec3 col;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
   gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(pos,1);
   col = vec3(1,1,0);
}
