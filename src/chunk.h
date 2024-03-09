#pragma once 

#include <array>
#include <cstdint>
#include <map>

#include "shape.h"

#define CHUNK_SIDE 32
#define CHUNK_SIDE_POW2 CHUNK_SIDE*CHUNK_SIDE
#define CHUNK_SIDE_POW3 CHUNK_SIDE*CHUNK_SIDE*CHUNK_SIDE


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
        void addFaceXPlane(glm::ivec3 position, bool reversed);
        void addFaceYPlane(glm::ivec3 position, bool reversed);
        void addFaceZPlane(glm::ivec3 position, bool reversed);
    public:
        Chunk(glm::ivec3 position);
        ~Chunk();

        uint8_t getBlock(glm::ivec3 position);

        void bind();
        void unbind();

        void generate();
        void draw();
};

