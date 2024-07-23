#version 330 core

in vec2 v_uvs;
in vec3 v_normal;

uniform vec3 u_color;
uniform vec3 u_lightDir;
uniform sampler2D u_texture;

out vec4 out_color;

void main() {
    // vec3 color = vec3(0.3f, 0.72f, 1.2f); 
    float light = dot(normalize(v_normal), normalize(u_lightDir));
    light += 1.0f;
    light *= 0.5f;
    light = 0.8 * sqrt(light) + 0.18;
    vec4 color = texture(u_texture, v_uvs);
    out_color = color * light;
    // out_color = vec4(vec3(light), 1.0);
}
