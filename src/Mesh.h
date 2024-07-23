#pragma once

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "GLCommon.h"

#include <vector>
#include <glm/glm.hpp>


struct VBLElement {
    int type;
    char normalized;
    unsigned int count;

    VBLElement(int type, char normalized, unsigned int count) 
        : type(type), normalized(normalized), count(count) {}
};

struct VBL {
    std::vector<VBLElement> elements;
    int stride = 0;
    int numAttribs = 0;

    VBL() 
        : elements(std::vector<VBLElement>()) {}

    void push(VBLElement elem) {
        elements.push_back(elem);
        stride += elem.count * GLSizeof(elem.type);
        numAttribs += elem.count;
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
        return getLayout().numAttribs;
    }
    static int getStride() {
        return getLayout().stride;
    }
    static VBL getLayout() {
        VBL layout = VBL();
        layout.pushf(3);
        layout.pushf(2);
        layout.pushf(1);
        return layout;
    }
};

struct Mesh {
    std::vector<float> vertices_;
    std::vector<unsigned int> indices_;

private:
    VBL layout_; 
    VAO vao_;
    VBO vbo_;
    EBO ebo_;

public:
    Mesh();
    ~Mesh();

    void bind();
    void unbind();

    void draw();
    void drawInstanced(unsigned int count);

public:
    void addCube(glm::vec3 position);
    
    void upload();
};
