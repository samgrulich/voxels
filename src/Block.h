#pragma once
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

class Block {
    public:
        glm::vec3 position_;
    public:
        Block(glm::vec3 position); 
        ~Block();
};

class Blocks {
    private: 
        VAO VAO_;
        VBO VBO_;
        EBO EBO_;
        std::vector<float> vertices_;
        std::vector<unsigned int> indices_;
    public:
        Blocks();
        ~Blocks();
    public:
        void add(Block &block);
        void upload();
        void draw(ShaderProgram& shaderProgram);
};
