#pragma once 

#include "Shader.h"
#include "VAO.h"
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Shape.h"

#define CHUNK_SIDE 32
#define CHUNK_SIDE_POW2 CHUNK_SIDE*CHUNK_SIDE
#define CHUNK_SIDE_POW3 CHUNK_SIDE*CHUNK_SIDE*CHUNK_SIDE


struct Node {
    Node* children[8] = {nullptr};
    bool isLeaf = false;
    uint8_t data = 0;

    ~Node() { }
};

class Chunk {
    public:
        glm::ivec3 position_; // chunk's position in the world (mod chunk_side)
        glm::ivec3 offset_; // chunk's offset from (0, 0, 0) in blocks
        bool isVisible_ = false;
        bool isActive_  = false;
        bool toUpload_  = false;
    private:
        int size_;
        int depth_;
        Node* root_;

        VAO VAO_;
        VBO VBO_;
        std::vector<float> vertices_;
        int faces = 0;

    private:
        // insert data to given node // todo - doc
        void insert(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth);
        // insert data to given node // todo - doc
        void insertTo(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth, int maxDepth);
        // get data stored at node
        uint8_t get(Node** node, glm::ivec3 point, glm::ivec3 position, int depth);
        // creates face and adds it into the vertices vector
        void generateFace(glm::ivec3 position, uint8_t id, unsigned int faceIndex);
        void addFaceXPlane(glm::ivec3 position, bool reversed);
        void addFaceYPlane(glm::ivec3 position, bool reversed);
        void addFaceZPlane(glm::ivec3 position, bool reversed);
    public:
        Chunk();
        Chunk(glm::ivec3 position);
        ~Chunk();

        // set block at position
        void setBlock(glm::ivec3 position, uint8_t value);
        // get block data by position (that is definetely inside the chunk)
        uint8_t getBlock(glm::ivec3 position);

        // Generate all blocks in the chunk
        void generate();
        // generate vertices and add them to the vertices list
        void mesh();
        // recreate the mesh
        void remesh();
        // draw the chunk
        void draw(ShaderProgram& shaderProgram);
};

inline std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunks;

