#version 330 core

// in vec2 v_uvs;

out vec4 out_color;

void main() {
    // vec2 position = v_uvs - vec2(0.5, 0.5);
    // float distance_from_middle = length(position);
    // float alpha = 1.0 - smoothstep(0.45, 0.5, distance_from_middle);
    //
    // vec3 color = vec3(0.3f, 0.72f, 1.2f); 
    // color *= smoothstep(0.5, 0.46, distance_from_middle);
    //
    // out_color = vec4(color, alpha);
    // out_color = vec4(v_uvs, 0.0, 1.0);
    vec3 color = vec3(0.3f, 0.72f, 1.2f); 
    out_color = vec4(color, 1.0);
}
