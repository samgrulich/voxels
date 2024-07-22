#pragma once

#include <GL/glew.h>
#include "VBO.h"

struct Attribute {
    int type;
    GLuint numComponents;
};

/**
 * @brief Represents a Vertex Array Object
 * and makes less manual adding and enabling attributes
 */
struct VAO {
private:
    VBO* vbo_ = nullptr;
    unsigned int stride_ = 0;
    std::vector<Attribute> attributes_;
public:
    GLuint ID;
public:
    VAO();
    ~VAO() {remove();}

    /**
     * @brief Set the Vbo object
     */
    void setVbo(VBO *vbo); 
    /**
     * @brief Add an attribute to the VAO (!after all atributes are added you need to enable them!)
     * @param type: type of the attribute 
     * @param numComponents: number of components of the attribute
     */
    void addAttrib(int type, GLuint numComponents);
    /**
     * @brief Enable the attributes of the VAO
     */
    void enableAttribs();
    // Binds the VAO
    void bind();
    // Unbind the VAO 
    void unbind();
    // Deletes the VAO
    void remove();
};
