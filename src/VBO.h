#pragma once 

#include <vector>
#include <GL/glew.h>

struct VBO {
    GLuint ID;

    /*
     * Creates a VBO and populates the buffer with vertices
     *
     * (note that the VBO needs to be deleted 
     *   by user by calling Delete())
     */
    VBO(std::vector<GLuint> vertices);
    /*
     * Creates empty VBO
     *
     * (note that the VBO needs to be deleted 
     *   by user by calling Delete())
     */
    VBO();
    ~VBO() {remove();}

    // // (Re)write new vertices to the VBO
    // void set(std::vector<GLfloat> vertices);
    // (Re)write new vertices to the VBO
    void set(std::vector<GLuint> vertices);
    // Binds the VBO
    void bind();
    // Unbinds the VBO
    void unbind();
    // Deletes the VBO 
    void remove();
};

