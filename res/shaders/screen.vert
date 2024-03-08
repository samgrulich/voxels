#version 330 core

layout(location=0) in vec4 pos;
layout(location=1) in vec2 uv;

out vec2 v_uvs;

void main() {
    vec3 position = pos.xyz;
    gl_Position = vec4(position, 1);
    v_uvs = uv;
}
