#include "Mesh.h"
#include <iostream>

Mesh::Mesh() {
    vertices_ = std::vector<float>();
    indices_ = std::vector<unsigned int>();
}

Mesh::~Mesh() { }

void Mesh::bind() {
    vao_.bind();
}

void Mesh::unbind() {
    vbo_.unbind();
    vao_.unbind();
}

void Mesh::draw() {
    bind();
    GLCall(glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, NULL));
}

void Mesh::drawInstanced(unsigned int count) {
    bind();
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, NULL, count));
}

void Mesh::addCube(glm::vec3 position) {
    float x = position.x;
    float y = position.y;
    float z = position.z;

    unsigned int io = vertices_.size() / Vertex::getSize();
        
    std::vector<float> vertices = {
        -0.5f+x, -0.5f+y, -0.5f+z, 0.0, 0.0,  // F0.5font Face
         0.5f+x, -0.5f+y, -0.5f+z, 1.0, 0.0,  // F0.5font Face
         0.5f+x,  0.5f+y, -0.5f+z, 1.0, 1.0,  // F0.5font Face
        -0.5f+x,  0.5f+y, -0.5f+z, 0.0, 1.0,  // F0.5font Face

         0.5f+x, -0.5f+y,  0.5f+z, 0.0, 0.0,  // Back Face
        -0.5f+x, -0.5f+y,  0.5f+z, 1.0, 0.0,  // Back Face
        -0.5f+x,  0.5f+y,  0.5f+z, 1.0, 1.0,  // Back Face
         0.5f+x,  0.5f+y,  0.5f+z, 0.0, 1.0,  // Back Face

        -0.5f+x, -0.5f+y,  0.5f+z, 0.0, 0.0,  // Left Face
        -0.5f+x, -0.5f+y, -0.5f+z, 1.0, 0.0,  // Left Face
        -0.5f+x,  0.5f+y, -0.5f+z, 1.0, 1.0,  // Left Face
        -0.5f+x,  0.5f+y,  0.5f+z, 0.0, 1.0,  // Left Face

         0.5f+x, -0.5f+y, -0.5f+z, 0.0, 0.0,  // Right Face
         0.5f+x, -0.5f+y,  0.5f+z, 1.0, 0.0,  // Right Face
         0.5f+x,  0.5f+y,  0.5f+z, 1.0, 1.0,  // Right Face
         0.5f+x,  0.5f+y, -0.5f+z, 0.0, 1.0,  // Right Face

        -0.5f+x,  0.5f+y, -0.5f+z, 0.0, 0.0,  // Top Face
         0.5f+x,  0.5f+y, -0.5f+z, 1.0, 0.0,  // Top Face
         0.5f+x,  0.5f+y,  0.5f+z, 1.0, 1.0,  // Top Face
        -0.5f+x,  0.5f+y,  0.5f+z, 0.0, 1.0,  // Top Face

        -0.5f+x, -0.5f+y,  0.5f+z, 0.0, 0.0,  // Bottom Face
         0.5f+x, -0.5f+y,  0.5f+z, 1.0, 0.0,  // Bottom Face
         0.5f+x, -0.5f+y, -0.5f+z, 1.0, 1.0,  // Bottom Face
        -0.5f+x, -0.5f+y, -0.5f+z, 0.0, 1.0   // Bottom Face
    };
    
    std::vector<unsigned int> indices = {
        0+io,  1+io,  2+io,  2+io,  3+io,  0+io,  // Front Face
        4+io,  5+io,  6+io,  6+io,  7+io,  4+io,  // Back Face
        8+io,  9+io,  10+io, 10+io, 11+io, 8+io,  // Left Face
        12+io, 13+io, 14+io, 14+io, 15+io, 12+io,  // Right Face
        16+io, 17+io, 18+io, 18+io, 19+io, 16+io,  // Top Face
        20+io, 21+io, 22+io, 22+io, 23+io, 20+io   // Bottom Face
    };

    vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
    indices_.insert(indices_.end(), indices.begin(), indices.end());
}

void Mesh::upload() {
    // // print vertices 
    // std::cout << "Vertices: ";
    // for (int i = 0; i < vertices_.size(); i++) {
    //     std::cout << vertices_[i] << " ";
    // }
    // std::cout << std::endl;

    vbo_.set(vertices_);
    
    layout_ = Vertex::getLayout();
    layout_.enableAttribs();

    ebo_.set(indices_);
}
