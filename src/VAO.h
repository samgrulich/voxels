#pragma once

#include <GL/glew.h>
#include "VBO.h"


struct VAO {
public:
    GLuint ID;
public:
    VAO();
    ~VAO() {remove();}

    // Binds the VAO
    void bind();
    // Unbind the VAO 
    void unbind();
    // Deletes the VAO
    void remove();
};
