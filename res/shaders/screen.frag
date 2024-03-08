#version 330 core

in vec2 v_uvs;

out vec4 out_color;

void main() {
    out_color = vec4(0, v_uvs.y, 0, 0.0);
}
