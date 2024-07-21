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
    EBO(std::vector<unsigned int> indices);
    /*
     * Creates empty VBO
     *
     * (note that the VBO needs to be deleted 
     *   by user by calling Delete())
     */
    EBO();
    ~EBO() {remove();}

    // (Re)write new vertices to the VBO
    void set(std::vector<GLuint> vertices);
    // Binds the VBO
    void bind();
    // Unbinds the VBO
    void unbind();
    // Deletes the VBO 
    void remove();
};
