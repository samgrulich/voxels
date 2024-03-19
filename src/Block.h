#pragma once 

#include <glm/glm.hpp>


typedef uint16_t BlockType;

struct Block {
    glm::vec3 position_;
    BlockType id;

    Block() {}
};
