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


class World {
public:
    World() {}

    /**
     * @brief Queries for chunk at position, if not found, creates a new one
     * @param x global position x
     * @param y global position y
     * @param z global position z
     * @return Block
     */
    static Block getBlock(int x, int y, int z);
    /**
     * @brief Sets the block at position, if not found, creates a new one
     * @param x global position x
     * @param y global position y
     * @param z global position z
     * @param block Block to set
     * @return Block
     */
    static void setBlock(int x, int y, int z, Block block);
    static void removeBlock(int x, int y, int z);
    static glm::ivec3 getChunkPosition(glm::vec3 pos);
    static glm::ivec3 getBlockPosition(glm::vec3 pos);
};

class Chunk {
    unsigned int vao_, vbo_, ibo_;
    std::vector<float> vertices_;
    std::vector<unsigned int> indices_;
    glm::ivec3 position_;
    Block* blocks_;

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
    /**
     * @brief Checks if the block is in this chunk, else queries the world
     * @param lx Local x position 
     * @param ly Local y position 
     * @param lz Local z position 
     * @param wx World x position 
     * @param wy World y position 
     * @param wz World z position 
     * @return Block 
     */
    const Block getBlock(int lx, int ly, int lz, int wx, int wy, int wz) const;
    /**
     * @brief Sets the block in this chunk, else fails
     * @param lx Local x position 
     * @param ly Local y position 
     * @param lz Local z position 
     * @param block Block to set
     */
    void setBlock(int lx, int ly, int lz, Block block);
    bool isNull() const;
private:
    void addFaceXPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void addFaceYPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void addFaceZPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void uploadMesh();
    int getBlockIndex(int lx, int ly, int lz) const;
};

