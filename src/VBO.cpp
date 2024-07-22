#include "VBO.h"


VBO::VBO(std::vector<GLuint> vertices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);;
}

VBO::VBO() {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::remove() {
    glDeleteBuffers(1, &ID);
}

void VBO::set(std::vector<float> vertices) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
}

void VBO::set(std::vector<GLuint> vertices) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
}

void VBO::set(float vertices[], int size) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
}
