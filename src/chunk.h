#pragma once 

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "shape.h"

#define CHUNK_SIDE 32
#define CHUNK_SIDE_POW2 CHUNK_SIDE*CHUNK_SIDE
#define CHUNK_SIDE_POW3 CHUNK_SIDE*CHUNK_SIDE*CHUNK_SIDE


class World;

struct Node {
    Node* children[8] = {nullptr};
    bool isLeaf = false;
    uint8_t data = 0;

    ~Node() { }
};

class Chunk {
    public:
        glm::ivec3 position_;
    private:
        int size_;
        int depth_;
        Node* root_;
        std::vector<float> vertices_;
        std::vector<unsigned int> indices_;

        Vbl layout_;
        unsigned int VAO_;
        unsigned int VB_;
        unsigned int IB_;
        int faces = 0;
    private:
        void addFaceXPlane(glm::ivec3 position, bool reversed);
        void addFaceYPlane(glm::ivec3 position, bool reversed);
        void addFaceZPlane(glm::ivec3 position, bool reversed);

        bool isPosOutside(glm::ivec3 position);
        glm::ivec3 posToWorld(glm::ivec3 position);

        void insert(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth);
        void insertTo(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth, int maxDepth);
        uint8_t get(Node** node, glm::ivec3 point, glm::ivec3 position, int depth);
    public:
        Chunk(glm::ivec3 position);
        Chunk();
        ~Chunk();

        void setBlock(glm::ivec3 position, uint8_t value);
        uint8_t getBlock(glm::ivec3 position);

        void bind();
        void unbind();

        void generate(World& world);
        void draw();
};


struct IVec3Comparator {
    bool operator()(const glm::ivec3& a, const glm::ivec3& b) const {
        return std::hash<glm::ivec3>()(a) < std::hash<glm::ivec3>()(b);
    }
};

class World {
    private:
        int viewDistance_ = 1;
        int fullViewDistance_ = 3;
        std::map<glm::ivec3, Chunk, IVec3Comparator> chunks_;
        std::map<glm::ivec3, Chunk*, IVec3Comparator> generated_;
        std::queue<Chunk*> toGenerate_;
        Chunk* lastChunk_;
        glm::ivec3 start_;
        glm::ivec3 end_;
        glm::ivec3 playerPos_;
    private:
        bool isPositionOutside(glm::ivec3 position);

        glm::ivec3 getChunkPosition(glm::ivec3 position);
        glm::ivec3 getBlockPosition(glm::ivec3 position);

        size_t getChunkIndex(glm::ivec3 position);
        size_t getBlockIndex(glm::ivec3 position);

        // adds positions of chunks in given X plane
        void addChunksXPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset);
        // adds positions of chunks in given Y plane
        void addChunksYPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset);
        // adds positions of chunks in given Z plane
        void addChunksZPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset);

        void loadChunks(std::vector<glm::ivec3>& chunkPositions);
        void unloadChunks(std::vector<glm::ivec3>& chunkPositions);
    public:
        World(glm::ivec3 playerPos);
        ~World();

        uint8_t getBlock(glm::ivec3 position);
        void generateChunk();
        void meshChunk();

        void updateRegion(glm::ivec3 camPos);
        void draw();
};
