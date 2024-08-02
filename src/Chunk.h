#pragma once
#include <glm/glm.hpp>
#include <mutex>
#include <queue>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <vector>

struct Block {
    unsigned int id;
    bool opaque;
};

namespace Blocks {
    static const Block AIR = {0, false};
    static const Block STONE = {1, true};
}


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
    bool dirty = true;
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
    void generateChunk();
    glm::ivec3 getChunkPos() const;
private:
    void addFaceXPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void addFaceYPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void addFaceZPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted);
    void uploadMesh();
    inline int getBlockIndex(int lx, int ly, int lz) const;
};


class World {
    glm::ivec3 center_;
    int radius_;
    bool loaded_ = false;
    
    std::mutex loadMutex_;
    std::queue<Chunk*> chunksToLoad_;
    std::queue<Chunk*> chunksToMesh_;
    std::mutex remeshMutex_;
    std::queue<Chunk*> chunksToRemesh_;

public:
    World(int radius) 
     : radius_(radius) {}


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
    static void drawChunks();
public:
    void loadArea(glm::ivec3 center, int radius);
    void loadChunks();
    void remeshChunks();
    void flagNeighbors(glm::ivec3 chunkPos, glm::ivec3 delta);
    void flagChunk(glm::ivec3 chunkPos);
};


Block generateBlock(int x, int y, int z);
