#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <vector>

struct Block {
    unsigned int id;
    bool opaque;
};

static std::unordered_map<glm::vec3, Block> blocks;

class World {
public:
    World() {}

    static Block getBlock(int x, int y, int z);
    static Block getBlock(glm::vec3 pos);
    static Block setBlock(int x, int y, int z, Block block);
    static Block setBlock(glm::vec3 pos, Block block);
    static Block removeBlock(int x, int y, int z);
};

class Chunk {
    unsigned int vao_, vbo_, ibo_;
    std::vector<float> vertices_;
    std::vector<unsigned int> indices_;
    glm::ivec3 position_;

    static const uint8_t ADJACENT_BITMASK_POS_X = 0b00000001;
    static const uint8_t ADJACENT_BITMASK_NEG_X = 0b00000010;
    static const uint8_t ADJACENT_BITMASK_POS_Y = 0b00000100;
    static const uint8_t ADJACENT_BITMASK_NEG_Y = 0b00001000;
    static const uint8_t ADJACENT_BITMASK_POS_Z = 0b00010000;
    static const uint8_t ADJACENT_BITMASK_NEG_Z = 0b00100000;
public:
    Chunk();
    Chunk(glm::ivec3 position);
    ~Chunk();
    
    void draw();
    void remesh();
    void setPosition(glm::ivec3 position);

private:
    void addFaceXPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void addFaceYPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void addFaceZPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void uploadMesh();
};

