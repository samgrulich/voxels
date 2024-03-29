#version 330 core

layout(location=0) in vec4 pos;
layout(location=1) in vec2 uv;

uniform mat4 u_MVP;

out vec2 v_uvs;
out vec3 v_col;

void main() {
    // vec3 instanceoffset = u_offsets[gl_InstanceID];
    // vec3 center = u_offset + instanceoffset * 0.5;
    // float size = 0.5;
    // if (gl_InstanceID == 12) {
    //     size = u_size;
    // }
    v_uvs = uv;
    vec3 vertexPosition = pos.xyz;
    v_col = vec3(0);
    if (mod(vertexPosition.x, 32.f) == 0 || mod(vertexPosition.z, 32.f) == 0)
        v_col.g = 1.0;

    gl_Position = u_MVP * vec4(vertexPosition, 1.0);
    // gl_Position = vec4(vertexPosition, 1.0);
}
