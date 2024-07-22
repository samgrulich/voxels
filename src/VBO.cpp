#include "VBO.h"
#include "GLCommon.h"


VBO::VBO(std::vector<float> vertices) {
    GLCall(glGenBuffers(1, &ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW));
}

VBO::VBO() {
    GLCall(glGenBuffers(1, &ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

void VBO::bind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

void VBO::unbind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VBO::remove() {
    GLCall(glDeleteBuffers(1, &ID));
}

void VBO::set(std::vector<float> vertices) {
    bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW));
}

void VBO::set(std::vector<GLuint> vertices) {
    bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW));
}

void VBO::set(float vertices[], int size) {
    bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertices[0]), vertices, GL_STATIC_DRAW));
}
