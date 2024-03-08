#version 330 core

layout(location=0) in vec4 pos;
layout(location=1) in vec2 uv;

// uniform mat4 u_MVP;
// uniform vec3 u_offset;
// uniform vec3 u_offsets[2500];
// uniform vec3 u_camup;
// uniform vec3 u_camright;
// uniform float u_size;
//
// out vec2 v_uvs;

void main() {
    // vec3 instanceoffset = u_offsets[gl_InstanceID];
    // vec3 center = u_offset + instanceoffset * 0.5;
    // float size = 0.5;
    // if (gl_InstanceID == 12) {
    //     size = u_size;
    // }
    // vec3 vertexPosition = center + vec3(pos) * size;
    //     // - u_camright * pos.x * u_size
    //     // + u_camup * pos.y * u_size;
    // gl_Position = u_MVP * vec4(vertexPosition, 1);
    // v_uvs = uv;
    gl_Position = vec4(pos.xyz, 1.0);
}
