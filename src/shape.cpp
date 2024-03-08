#include <GL/glew.h>

#include "shape.h"
#include "glcommon.h"


Shape::Shape(std::vector<float> vertices, std::vector<unsigned int> indices)
    : m_vertices(vertices), m_indices(indices)
{
    // vao
    GLCall(glGenVertexArrays(1, &m_vao));
    GLCall(glBindVertexArray(m_vao));

    // vb
    GLCall(glGenBuffers(1, &m_vb));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vb));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*Vertex::get_size(), &vertices[0], GL_STATIC_DRAW));

    m_layout = Vertex::get_layout();
    m_layout.enableAttribs();

    // ib
    GLCall(glGenBuffers(1, &m_ib));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));
}

Shape::~Shape() {
    GLCall(glDeleteBuffers(1, &m_vao));
    GLCall(glDeleteBuffers(1, &m_vb));
    GLCall(glDeleteBuffers(1, &m_ib));
}

void Shape::bind() {
    GLCall(glBindVertexArray(m_vao));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib));
}

void Shape::unbind() {
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Shape::draw() {
    bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL))
}

void Shape::draw_instanced(unsigned int count) {
    bind();
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL, count));
}

Shape quad(float a) {
    float vertices_raw[] = {
        0.5f*a, -0.5f*a, 0.0, 1.0, 0.0, // 0
       -0.5f*a, -0.5f*a, 0.0, 0.0, 0.0, // 1
        0.5f*a,  0.5f*a, 0.0, 1.0, 1.0, // 2
       -0.5f*a,  0.5f*a, 0.0, 0.0, 1.0, // 3
    };
    unsigned int indices_raw[] = {
        0, 1, 2,
        1, 3, 2,
        // 0, 2, 1,
        // 1, 2, 3,
    };
    
    std::vector<float> vertices(vertices_raw, vertices_raw + sizeof vertices_raw / sizeof vertices_raw[0]);
    std::vector<unsigned int> indices(indices_raw, indices_raw + sizeof indices_raw / sizeof indices_raw[0]);

    return Shape(vertices, indices);
}

Shape rect(float w, float h) {
    float vertices_raw[] = {
        0.5f*w, -0.5f*h, 0.0, 1.0, 0.0, // 0
       -0.5f*w, -0.5f*h, 0.0, 0.0, 0.0, // 1
        0.5f*w,  0.5f*h, 0.0, 1.0, 1.0, // 2
       -0.5f*w,  0.5f*h, 0.0, 0.0, 1.0, // 3
    };
    unsigned int indices_raw[] = {
        0, 1, 2,
        1, 3, 2,
        // 0, 2, 1,
        // 1, 2, 3,
    };
    
    std::vector<float> vertices(vertices_raw, vertices_raw + sizeof vertices_raw / sizeof vertices_raw[0]);
    std::vector<unsigned int> indices(indices_raw, indices_raw + sizeof indices_raw / sizeof indices_raw[0]);

    return Shape(vertices, indices);
}

Shape circle(float r) {
    float vertices_raw[] = {
        // Format: x * r, y * r, z * r, u, v
        0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
        0.5f * r, 0.0f, 0.0f, 1.0f, 0.5f,
        0.4924038765f * r, 0.08682408883f * r, 0.0f, 0.9924038765f, 0.58682408883f,
        0.46984631039f * r, 0.17101007166f * r, 0.0f, 0.96984631039f, 0.67155895485f,
        0.43301270189f * r, 0.25f * r, 0.0f, 0.93301270189f, 0.75f,
        0.38302222156f * r, 0.32139380484f * r, 0.0f, 0.88302222156f, 0.82139380484f,
        0.32139380484f * r, 0.38302222156f * r, 0.0f, 0.82039380484f, 0.88302222156f,
        0.25f * r, 0.43301270189f * r, 0.0f, 0.75f, 0.93301270189f,
        0.17155895485f * r, 0.46984631039f * r, 0.0f, 0.67155895485f, 0.96984631039f,
        0.08682408883f * r, 0.4924038765f * r, 0.0f, 0.58682408883f, 0.9924038765f,
        3.06161699787e-17f * r, 0.5f * r, 0.0f, 0.5f, 1.0f,
        -0.08682408883f * r, 0.4924038765f * r, 0.0f, 0.41317591117f, 0.9924038765f,
        -0.17101007166f * r, 0.46984631039f * r, 0.0f, 0.32844104515f, 0.96984631039f,
        -0.25f * r, 0.43301270189f * r, 0.0f, 0.25f, 0.93301270189f,
        -0.32139380484f * r, 0.38302222156f * r, 0.0f, 0.17860619516f, 0.88302222156f,
        -0.38302222156f * r, 0.32139380484f * r, 0.0f, 0.11797777844f, 0.82139380484f,
        -0.43301270189f * r, 0.25f * r, 0.0f, 0.06698729811f, 0.75f,
        -0.46984631039f * r, 0.17155895485f * r, 0.0f, 0.03015368961f, 0.67155895485f,
        -0.4924038765f * r, 0.08682408883f * r, 0.0f, 0.0075961235f, 0.58682408883f,
        -0.5f * r, 6.12323399574e-17f * r, 0.0f, 0.0f, 0.5f,
        -0.4924038765f * r, -0.08682408883f * r, 0.0f, 0.0075961235f, 0.41317591117f,
        -0.46984631039f * r, -0.17101007166f * r, 0.0f, 0.03015368961f, 0.32844104515f,
        -0.43301270189f * r, -0.25f * r, 0.0f, 0.06698729811f, 0.25f,
        -0.38302222156f * r, -0.32139380484f * r, 0.0f, 0.11797777844f, 0.17860619516f,
        -0.32139380484f * r, -0.38302222156f * r, 0.0f, 0.17860619516f, 0.11797777844f,
        -0.25f * r, -0.43301270189f * r, 0.0f, 0.25f, 0.06698729811f,
        -0.17155895485f * r, -0.46984631039f * r, 0.0f, 0.32844104515f, 0.03015368961f,
        -0.08682408883f * r, -0.4924038765f * r, 0.0f, 0.41317591117f, 0.0075961235f,
        -9.18485099361e-17f * r, -0.5f * r, 0.0f, 0.5f, 0.0f,
        0.08682408883f * r, -0.4924038765f * r, 0.0f, 0.58682408883f, 0.0075961235f,
        0.17101007166f * r, -0.46984631039f * r, 0.0f, 0.67155895485f, 0.03015368961f,
        0.25f * r, -0.43301270189f * r, 0.0f, 0.75f, 0.06698729811f,
        0.32139380484f * r, -0.38302222156f * r, 0.0f, 0.82139380484f, 0.11797777844f,
        0.38302222156f * r, -0.32139380484f * r, 0.0f, 0.88302222156f, 0.17860619516f,
        0.43301270189f * r, -0.25f * r, 0.0f, 0.93301270189f, 0.25f,
        0.46984631039f * r, -0.17155895485f * r, 0.0f, 0.96984631039f, 0.32844104515f,
        0.4924038765f * r, -0.08682408883f * r, 0.0f, 0.9924038765f, 0.41317591117f
    };
    
    unsigned int indices_raw[] = {
        0, 2, 1,
        0, 3, 2,
        0, 4, 3,
        0, 5, 4,
        0, 6, 5,
        0, 7, 6,
        0, 8, 7,
        0, 9, 8,
        0, 10, 9,
        0, 11, 10,
        0, 12, 11,
        0, 13, 12,
        0, 14, 13,
        0, 15, 14,
        0, 16, 15,
        0, 17, 16,
        0, 18, 17,
        0, 19, 18,
        0, 20, 19,
        0, 21, 20,
        0, 22, 21,
        0, 23, 22,
        0, 24, 23,
        0, 25, 24,
        0, 26, 25,
        0, 27, 26,
        0, 28, 27,
        0, 29, 28,
        0, 30, 29,
        0, 31, 30,
        0, 32, 31,
        0, 33, 32,
        0, 34, 33,
        0, 35, 34,
        0, 1, 35
    };
    
    std::vector<float> vertices(vertices_raw, vertices_raw + sizeof vertices_raw / sizeof vertices_raw[0]);
    std::vector<unsigned int> indices(indices_raw, indices_raw + sizeof indices_raw / sizeof indices_raw[0]);

    return Shape(vertices, indices);
}

Shape cube(float a) {
    float r = a / 2;
    float vertices_raw[] = {
        -r, -r, -r, 0.0, 0.0,
        r, -r, -r, 1.0, 0.0,
        r, r, -r, 1.0, 1.0,
        -r, r, -r, 0.0, 1.0,
        -r, -r, r, 0.0, 0.0,
        r, -r, r, 1.0, 0.0,
        r, r, r, 1.0, 1.0,
        -r, r, r, 0.0, 1.0
    };
    unsigned int indices_raw[] = {
        0, 1, 2,
        2, 3, 0,
        1, 5, 6,
        6, 2, 1,
        7, 6, 5,
        5, 4, 7,
        4, 0, 3,
        3, 7, 4,
        4, 5, 1,
        1, 0, 4,
        3, 2, 6,
        6, 7, 3
    };

    std::vector<float> vertices(vertices_raw, vertices_raw + sizeof vertices_raw / sizeof vertices_raw[0]);
    std::vector<unsigned int> indices(indices_raw, indices_raw + sizeof indices_raw / sizeof indices_raw[0]);

    return Shape(vertices, indices);
}
