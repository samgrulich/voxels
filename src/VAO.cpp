#include "VAO.h"
#include "GLCommon.h"

VAO::VAO() {
    GLCall(glGenVertexArrays(1, &ID));
    GLCall(glBindVertexArray(ID));
}

// void VAO::setVbo(VBO *vbo) {
//     vbo_ = vbo;
// }
//
// void VAO::addAttrib(int type, GLuint numComponents) {
//     stride_ += numComponents;
//     attributes_.push_back({type, numComponents});
// }
//
// void VAO::enableAttribs() {
//     bind();
//     vbo_->bind();
//     int offset = 0;
//     for (int i = 0; i < attributes_.size(); i++) {
//         unsigned int type = attributes_[i].type;
//         unsigned int numComponents = attributes_[i].numComponents;
//         void* offset_size = (void*)(offset*sizeof(GLuint)); // convert the offset to the byte size (as void*)
//         offset += numComponents;
//         GLCall(glVertexAttribPointer(i, numComponents, type, GL_FALSE, stride_*sizeof(GLuint), offset_size));
//         GLCall(glEnableVertexAttribArray(i));
//     }
// }

void VAO::bind() {
    GLCall(glBindVertexArray(ID));
}

void VAO::unbind() {
    GLCall(glBindVertexArray(0));
}

void VAO::remove() {
    GLCall(glDeleteVertexArrays(1, &ID));
}
