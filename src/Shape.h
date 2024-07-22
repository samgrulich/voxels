#pragma once

#include <iostream>
#include <vector>

#include <glm/matrix.hpp>
#include <GL/glew.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "GLCommon.h"


struct VBLElement {
    int type;
    char normalized;
    unsigned int count;

    VBLElement(int type, char normalized, unsigned int count) 
        : type(type), normalized(normalized), count(count) {}
};

struct VBL {
    std::vector<VBLElement> elements;
    int stride;

    VBL() 
        : stride(0), elements(std::vector<VBLElement>()) {}

    void push(VBLElement elem) {
        elements.push_back(elem);
        stride += elem.count * GLSizeof(elem.type);
    }

    void pushf(unsigned int count) {
        push(VBLElement(GL_FLOAT, GL_FALSE, count));
    }
    
    void pushui(unsigned int count) {
        push(VBLElement(GL_UNSIGNED_INT, GL_FALSE, count));
    }
    
    void pushb(unsigned int count) {
        push(VBLElement(GL_UNSIGNED_BYTE, GL_TRUE, count));
    }

    void enableAttribs() {
        size_t offset = 0;
        for (int i = 0; i < elements.size(); i++) {
            VBLElement &element = elements[i];
            void* offset_size = (void*)(offset);
            offset += element.count * GLSizeof(element.type);
            GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, offset_size));
            GLCall(glEnableVertexAttribArray(i));
        }
    }
};

struct Vertex {
    static int getSize() {
        return getLayout().stride;
    }
    static VBL getLayout() {
        VBL layout = VBL();
        layout.pushf(3);
        layout.pushf(2);
        return layout;
    }
};

class Shape {
    private:
        std::vector<float> vertices_;
        std::vector<unsigned int> indices_;
        VBL layout_;
        VAO vao_;
        VBO vbo_;
        EBO ebo_;
        unsigned int VAO_;
        unsigned int VB_;
        unsigned int IB_;

    public:
        Shape();
        Shape(std::vector<float> vertices, std::vector<unsigned int> indices);
        ~Shape();

        void bind();
        void unbind();

        void draw();
        void drawInstanced(unsigned int count);
};

Shape quad(float a);
Shape rect(float w, float h);
Shape circle(float r);
Shape cube(float a);
