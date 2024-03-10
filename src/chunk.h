#pragma once 

#include <array>
#include <cstdint>
#include <map>
#include <vector>

#include "shape.h"

#define CHUNK_SIDE 32
#define CHUNK_SIDE_POW2 CHUNK_SIDE*CHUNK_SIDE
#define CHUNK_SIDE_POW3 CHUNK_SIDE*CHUNK_SIDE*CHUNK_SIDE


class World;

class Chunk {
    public:
        glm::ivec3 m_position;
        uint8_t m_data[CHUNK_SIDE_POW3];
    private:
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
    public:
        Chunk(glm::ivec3 position);
        ~Chunk();

        void bind();
        void unbind();

        void generate(World& world);
        void draw();
};

class World {
    private:
        unsigned int m_view_distance = 3;
        std::vector<Chunk> m_chunks = std::vector<Chunk>();
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
        void draw();
};
