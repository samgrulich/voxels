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
        glm::ivec3 m_position;
    private:
        int m_size;
        int m_depth;
        Node* m_root;
        std::vector<float> m_vertices;
        std::vector<unsigned int> m_indices;

        Vbl m_layout;
        unsigned int m_vao;
        unsigned int m_vb;
        unsigned int m_ib;
        int faces = 0;
    private:
        void add_face_x_plane(glm::ivec3 position, bool reversed);
        void add_face_y_plane(glm::ivec3 position, bool reversed);
        void add_face_z_plane(glm::ivec3 position, bool reversed);

        bool is_position_outside(glm::ivec3 position);
        glm::ivec3 position_to_world(glm::ivec3 position);

        void insert(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth);
        void insertTo(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth, int maxDepth);
        uint8_t get(Node** node, glm::ivec3 point, glm::ivec3 position, int depth);
    public:
        Chunk(glm::ivec3 position);
        Chunk();
        ~Chunk();

        void set_block(glm::ivec3 position, uint8_t value);
        uint8_t get_block(glm::ivec3 position);

        void bind();
        void unbind();

        void generate(World& world);
        void draw();
};


struct IVec3Comparator {
    size_t operator()(const glm::ivec3& k) const {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
    }
    bool operator()(const glm::ivec3& a, const glm::ivec3& b) const {
        return a == b;
    }
};

class World {
    private:
        unsigned int m_view_distance = 6;
        // todo: add caching to chunk retriving, add caching to chunk nodes
        // todo: center loaded area arround player, add loading unloading new chunks
        // todo: add multithreading
        std::map<glm::ivec3, Chunk, IVec3Comparator> m_chunks = std::map<glm::ivec3, Chunk, IVec3Comparator>();
        // std::vector<Chunk> m_chunks = std::vector<Chunk>();
        std::vector<Chunk*> m_generated = std::vector<Chunk*>();
        std::queue<Chunk*> m_to_generate = std::queue<Chunk*>();
        Chunk* m_last_chunk;
    private:
        bool is_position_outside(glm::ivec3 position);
        glm::ivec3 get_chunk_position(glm::ivec3 position);
        glm::ivec3 get_block_position(glm::ivec3 position);
        size_t get_chunk_index(glm::ivec3 position);
        size_t get_block_index(glm::ivec3 position);
    public:
        World();
        ~World();

        uint8_t get_block(glm::ivec3 position);
        void generate_chunk();
        void mesh_chunk();
        void draw();
};
