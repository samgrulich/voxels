#include "EBO.h"

EBO::EBO(std::vector<unsigned int> indices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
}

EBO::EBO() {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::remove() {
    glDeleteBuffers(1, &ID);
}

void EBO::set(std::vector<unsigned int> indices) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
}

void EBO::set(GLuint indices[], int size) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*sizeof(indices[0]), indices, GL_STATIC_DRAW);
}
