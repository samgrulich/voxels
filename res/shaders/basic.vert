#version 330 core

layout(location=0) in vec4 pos;
layout(location=1) in vec2 uvs;
layout(location=2) in float id;

uniform mat4 u_MVP;

out vec2 v_uvs;
out vec3 v_normal;

vec3 normals[] = vec3[](
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.0, -1.0),
    vec3(1.0, 0.0, 0.0),
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, -1.0, 0.0)
);

void main() {
    v_normal = normals[uint(id)];
    v_uvs = uvs;
    vec3 vertexPosition = pos.xyz;

    gl_Position = u_MVP * vec4(vertexPosition, 1.0);
    // gl_Position = vec4(vertexPosition, 1.0);
}
