#include "EBO.h"
#include "GLCommon.h"

EBO::EBO(std::vector<unsigned int> indices) {
    GLCall(glGenBuffers(1, &ID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), &indices[0], GL_STATIC_DRAW));
}

EBO::EBO() {
    GLCall(glGenBuffers(1, &ID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}

void EBO::bind() {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}

void EBO::unbind() {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void EBO::remove() {
    GLCall(glDeleteBuffers(1, &ID));
}

void EBO::set(std::vector<unsigned int> indices) {
    bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), &indices[0], GL_STATIC_DRAW));
}

void EBO::set(GLuint indices[], int size) {
    bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*sizeof(indices[0]), indices, GL_STATIC_DRAW));
}
