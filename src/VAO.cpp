#include "VAO.h"

VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void VAO::linkAttrib(VBO &vbo, GLuint layout, GLuint numComponents, GLsizeiptr stride, unsigned int offset) {
    void* offset_size = (void*)(offset*sizeof(GLuint)); // convert the offset to the byte size (as void*)
    bind();
    vbo.bind();
    glVertexAttribPointer(layout, numComponents, GL_UNSIGNED_INT, GL_FALSE, stride*sizeof(GLuint), offset_size);
    glEnableVertexAttribArray(layout);
}

void VAO::bind() {
    glBindVertexArray(ID);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::remove() {
    glDeleteVertexArrays(1, &ID);
}
