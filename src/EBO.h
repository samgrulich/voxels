#pragma once

#include <vector>
#include <GL/glew.h>

struct EBO {
    GLuint ID;

    /*
     * Creates a VBO and populates the buffer with vertices
     *
     * (note that the VBO needs to be deleted 
     *   by user by calling Delete())
     */
    EBO(std::vector<GLfloat> indices);
    /*
     * Creates empty VBO
     *
     * (note that the VBO needs to be deleted 
     *   by user by calling Delete())
     */
    EBO();
    ~EBO() {erase();}

    // (Re)write new vertices to the VBO
    void set(std::vector<GLuint> vertices);
    // Binds the VBO
    void bind();
    // Unbinds the VBO
    void unbind();
    // Deletes the VBO 
    void erase();
};
