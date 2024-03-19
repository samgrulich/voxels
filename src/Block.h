#pragma once 

#include <glm/glm.hpp>


typedef uint16_t BlockType;

enum BlockFace {
    Back = 0,
    Front = 1,
    Left = 2,
    Right = 3,
    Top = 4, 
    Bottom = 5
};

struct Block {
    glm::ivec3 position_;
    bool isTransparent;
    BlockType id;

    Block() {}
};
