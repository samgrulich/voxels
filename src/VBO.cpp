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

void VBO::erase() {
    glDeleteBuffers(1, &ID);
}

// void VBO::set(std::vector<GLfloat> vertices) {
//     bind();
//     glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
// }

void VBO::set(std::vector<GLuint> vertices) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
}
