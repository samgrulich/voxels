#pragma once

#include <vector>

#include <glm/matrix.hpp>
#include <GL/glew.h>

#include "glcommon.h"


struct VblElement {
    int type;
    char normalized;
    unsigned int count;

    VblElement(int type, char normalized, unsigned int count) 
        : type(type), normalized(normalized), count(count) {}
};

struct Vbl {
    std::vector<VblElement> elements;
    int stride;

    Vbl() 
        : stride(0), elements(std::vector<VblElement>()) {}

    void pushf(unsigned int count) {
        VblElement elem = VblElement(GL_FLOAT, GL_FALSE, count);
        elements.push_back(elem);
        stride += count * GLSizeof(elem.type);
    }
    
    void pushui(unsigned int count) {
        VblElement elem = VblElement(GL_UNSIGNED_INT, GL_FALSE, count);
        elements.push_back(elem);
        elements.push_back(elem);
        stride += count * GLSizeof(elem.type);
    }
    
    void pushb(unsigned int count) {
        VblElement elem = VblElement(GL_UNSIGNED_BYTE, GL_TRUE, count);
        elements.push_back(elem);
        stride += count * GLSizeof(elem.type);
    }

    void enableAttribs() {
        void * offset = 0;
        for (int i = 0; i < elements.size(); i++) {
            VblElement &element = elements[i];
            GLCall(glEnableVertexAttribArray(i));
            GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, offset));
            offset = (void*)((size_t)offset + element.count * GLSizeof(element.type));
        }
    }
};

struct Vertex {
    static int get_size() {
        return (3 + 2)*sizeof(float);
    }
    static Vbl get_layout() {
        Vbl layout = Vbl();
        layout.pushf(3);
        layout.pushf(2);
        return layout;
    }
};

class Shape {
    private:
        std::vector<float> m_vertices;
        std::vector<unsigned int> m_indices;
        Vbl m_layout;
        unsigned int m_vao;
        unsigned int m_vb;
        unsigned int m_ib;

    public:
        Shape(std::vector<float> vertices, std::vector<unsigned int> indices);
        ~Shape();

        void bind();
        void unbind();

        void draw();
        void draw_instanced(unsigned int count);
};

Shape quad(float a);
Shape rect(float w, float h);
Shape circle(float r);
Shape cube(float a);
