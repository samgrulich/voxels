#version 330 core

in vec2 v_uvs;
in vec3 v_col;

uniform vec3 u_color;
out vec4 out_color;

void main() {
    vec3 color = vec3(0.3f, 0.72f, 1.2f); 
    color = u_color;
    color = v_col;
    // color.xy = v_uvs.xy;
    out_color = vec4(color, 1.0);
}
