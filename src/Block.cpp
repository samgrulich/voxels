#include "Block.h"
#include <iostream>

Blocks::Blocks() : VAO_(), VBO_(), EBO_(), vertices_(), indices_(){ 
    VAO_.bind();
    VBO_.bind();
    EBO_.bind();
    VAO_.linkAttrib(VBO_, 0, 3, 5, 0);
    VAO_.linkAttrib(VBO_, 1, 2, 5, 3);
    VAO_.unbind();
}

Blocks::~Blocks() { }

Block::Block(glm::vec3 position) : position_(position) { }
Block::~Block() { }

void Blocks::add(Block &block) {
    unsigned int io = vertices_.size() / 5 * 4; // index offset
    glm::vec3 vo = block.position_; // vertex offset

    unsigned int indices[] = {
        0+io, 1+io, 3+io, 3+io, 1+io, 2+io,
        1+io, 5+io, 2+io, 2+io, 5+io, 6+io,
        5+io, 4+io, 6+io, 6+io, 4+io, 7+io,
        4+io, 0+io, 7+io, 7+io, 0+io, 3+io,
        3+io, 2+io, 7+io, 7+io, 2+io, 6+io,
        4+io, 5+io, 0+io, 0+io, 5+io, 1+io
    };

    float vertices[] = {
    	0.0f + vo.x, 0.0f + vo.y, 0.0f + vo.z, 0.0f, 0.0f,
    	1.0f + vo.x, 0.0f + vo.y, 0.0f + vo.z, 1.0f, 0.0f,
    	1.0f + vo.x, 1.0f + vo.y, 0.0f + vo.z, 1.0f, 1.0f,
    	0.0f + vo.x, 1.0f + vo.y, 0.0f + vo.z, 0.0f, 1.0f,
    	0.0f + vo.x, 0.0f + vo.y, 1.0f + vo.z, 0.0f, 0.0f,
    	1.0f + vo.x, 0.0f + vo.y, 1.0f + vo.z, 1.0f, 0.0f,
    	1.0f + vo.x, 1.0f + vo.y, 1.0f + vo.z, 1.0f, 1.0f,
    	0.0f + vo.x, 1.0f + vo.y, 1.0f + vo.z, 0.0f, 1.0f,
    };

    indices_.insert(indices_.end(), std::begin(indices), std::end(indices));
    vertices_.insert(vertices_.end(), std::begin(vertices), std::end(vertices));
    
    std::cout << "Indices: ";
    for (int i = 0; i < 36; i++) {
        std::cout << indices_[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Vertices: ";
    for (int i = 0; i < 40; i++) {
        std::cout << vertices_[i] << " ";
    }
    std::cout << std::endl;
}

void Blocks::upload() {
    VAO_.bind();
    VBO_.set(vertices_);
    EBO_.set(indices_);
    VAO_.unbind();
}

void Blocks::draw(ShaderProgram& shaderProgram) {
    shaderProgram.bind();
    VAO_.bind();
    glDrawElements(GL_TRIANGLES, vertices_.size(), GL_UNSIGNED_INT, 0);
    VAO_.unbind();
}
