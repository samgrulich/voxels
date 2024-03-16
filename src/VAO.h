#pragma once

#include <GL/glew.h>
#include "VBO.h"

struct VAO {
    GLuint ID;

    VAO();
    ~VAO() {erase();}

    /* Links a VBO attribute 
     * (the datatype of the attributes are uints by default)
     * @param vbo: vertex buffer, which contains the data
     * @param layout: index of the vertex attribute to be modified
     * @param numComponents: number of components
     * @param stride: size of vertex (number of uints)
     * @param offset: offset of the attrib from the start of vertex (number of uints)
     */
    void linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLsizeiptr stride, unsigned int offset);
    // Binds the VAO
    void bind();
    // Unbind the VAO 
    void unbind();
    // Deletes the VAO
    void erase();
};
