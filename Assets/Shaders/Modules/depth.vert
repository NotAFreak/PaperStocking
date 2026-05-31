#version 330

in vec3 vPos;

layout(std140) uniform CamData {
    mat4 camMat;
    vec3 camPos;
    vec2 screenSize;
};

uniform mat4 uModel;

void main() {
    gl_Position = camMat * uModel * vPos;
}