#include <cstdint>
#include <array>
#include <vector>

#include <GL/glew.h>

#include "chunk.h"
#include "glcommon.h"

#define DEFAULT_VERTICES_LEN 100
#define DEFAULT_INDICES_LEN 100

#define ADJACENT_BITMASK_POS_X 0x01u
#define ADJACENT_BITMASK_NEG_X 0x02u
#define ADJACENT_BITMASK_POS_Y 0x04u
#define ADJACENT_BITMASK_NEG_Y 0x08u
#define ADJACENT_BITMASK_POS_Z 0x10u
#define ADJACENT_BITMASK_NEG_Z 0x20u
#define ADJACENT_ALL_BITMASKS  0x3fu


Chunk::Chunk(glm::ivec3 position) 
    : m_position(position)
{
    m_vertices = std::vector<float>(0, DEFAULT_VERTICES_LEN);
    m_indices = std::vector<unsigned int>(0, DEFAULT_INDICES_LEN);

    GLCall(glGenVertexArrays(1, &m_vao));
    GLCall(glBindVertexArray(m_vao));
    
    GLCall(glGenBuffers(1, &m_vb));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vb));

    m_layout = Vertex::get_layout();
    m_layout.enableAttribs();

    GLCall(glGenBuffers(1, &m_ib));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib));
    
    for (int i = 0; i < CHUNK_SIDE_POW3; i++) {
        m_data[i] = 1u;
    }
}

Chunk::~Chunk() {
    GLCall(glDeleteBuffers(1, &m_vao));
    GLCall(glDeleteBuffers(1, &m_vb));
    GLCall(glDeleteBuffers(1, &m_ib));
}

void Chunk::bind() {
    GLCall(glBindVertexArray(m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vb));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib));
}

void Chunk::unbind() {
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Chunk::addFaceXPlane(glm::ivec3 position, bool reversed) {
    unsigned int index_offset = faces++*4;
    unsigned int indices[] = {
        0u+index_offset, 1u+index_offset, 2u+index_offset,
        1u+index_offset, 3u+index_offset, 2u+index_offset,
    };
    unsigned int indices_reversed[] = {
        0u+index_offset, 2u+index_offset, 1u+index_offset,
        1u+index_offset, 2u+index_offset, 3u+index_offset,
    };

    glm::vec3 vertex_offset = position + m_position * CHUNK_SIDE;
    float vertices[] = {
        1.0f + vertex_offset.x,  0.0f + vertex_offset.y, 0.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x,  0.0f + vertex_offset.y, 0.0f + vertex_offset.z, 0.0, 0.0, // 1
        1.0f + vertex_offset.x,  1.0f + vertex_offset.y, 0.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x,  1.0f + vertex_offset.y, 0.0f + vertex_offset.z, 0.0, 1.0, // 3
    };

    m_vertices.insert(m_vertices.end(), std::begin(vertices), std::end(vertices));
    if (!reversed)
        m_indices.insert(m_indices.end(), std::begin(indices), std::end(indices));
    else 
        m_indices.insert(m_indices.end(), std::begin(indices_reversed), std::end(indices_reversed));
}

void Chunk::addFaceYPlane(glm::ivec3 position, bool reversed) {
    unsigned int index_offset = faces++*4;
    unsigned int indices[] = {
        0u+index_offset, 1u+index_offset, 2u+index_offset,
        1u+index_offset, 3u+index_offset, 2u+index_offset,
    };
    unsigned int indices_reversed[] = {
        0u+index_offset, 2u+index_offset, 1u+index_offset,
        1u+index_offset, 2u+index_offset, 3u+index_offset,
    };

    glm::vec3 vertex_offset = position + m_position * CHUNK_SIDE;
    float vertices[] = {
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y, -1.0f + vertex_offset.z, 0.0, 0.0, // 1
        0.0f + vertex_offset.x, 1.0f + vertex_offset.y,  0.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x, 1.0f + vertex_offset.y, -1.0f + vertex_offset.z, 0.0, 1.0, // 3
    };

    m_vertices.insert(m_vertices.end(), std::begin(vertices), std::end(vertices));
    if (!reversed)
        m_indices.insert(m_indices.end(), std::begin(indices), std::end(indices));
    else 
        m_indices.insert(m_indices.end(), std::begin(indices_reversed), std::end(indices_reversed));
}

void Chunk::addFaceZPlane(glm::ivec3 position, bool reversed) {
    unsigned int index_offset = faces++*4;
    unsigned int indices[] = {
        0u+index_offset, 1u+index_offset, 2u+index_offset,
        1u+index_offset, 3u+index_offset, 2u+index_offset,
    };
    unsigned int indices_reversed[] = {
        0u+index_offset, 2u+index_offset, 1u+index_offset,
        1u+index_offset, 2u+index_offset, 3u+index_offset,
    };
    
    glm::vec3 vertex_offset = position + m_position * CHUNK_SIDE;
    float vertices[] = {
        1.0f + vertex_offset.x, 0.0f + vertex_offset.y, -1.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y, -1.0f + vertex_offset.z, 0.0, 0.0, // 1
        1.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 0.0, 1.0, // 3
    };

    m_vertices.insert(m_vertices.end(), std::begin(vertices), std::end(vertices));
    if (!reversed)
        m_indices.insert(m_indices.end(), std::begin(indices), std::end(indices));
    else 
        m_indices.insert(m_indices.end(), std::begin(indices_reversed), std::end(indices_reversed));
}

void Chunk::generate() {
    for (int z = 0; z < CHUNK_SIDE; z++) {
        for (int y = 0; y < CHUNK_SIDE; y++) {
            for (int x = 0; x < CHUNK_SIDE; x++) {
                uint8_t opaqueBitmask = 0x0;
                int mX = x, mY = y, mZ = z;
                uint8_t b =   getBlock({mX, mY, mZ});
                uint8_t bpx = getBlock({mX+1, mY, mZ});
                uint8_t bnx = getBlock({mX-1, mY, mZ});
                uint8_t bpy = getBlock({mX, mY+1, mZ});
                uint8_t bny = getBlock({mX, mY-1, mZ});
                uint8_t bpz = getBlock({mX, mY, mZ+1});
                uint8_t bnz = getBlock({mX, mY, mZ-1});

                if (b == 0)
                    continue;

                opaqueBitmask |= bpx!=0 ? ADJACENT_BITMASK_POS_X : 0;
                opaqueBitmask |= bnx!=0 ? ADJACENT_BITMASK_NEG_X : 0;
                opaqueBitmask |= bpy!=0 ? ADJACENT_BITMASK_POS_Y : 0;
                opaqueBitmask |= bny!=0 ? ADJACENT_BITMASK_NEG_Y : 0;
                opaqueBitmask |= bpz!=0 ? ADJACENT_BITMASK_POS_Z : 0;
                opaqueBitmask |= bnz!=0 ? ADJACENT_BITMASK_NEG_Z : 0;

                if (opaqueBitmask == ADJACENT_ALL_BITMASKS)
                    continue;

                if ((opaqueBitmask & ADJACENT_BITMASK_POS_X) == 0)
                    addFaceYPlane({x+1, y, z}, true);
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_X) == 0)
                    addFaceYPlane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_POS_Y) == 0)
                    addFaceZPlane({x, y+1, z}, true);
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_Y) == 0)
                    addFaceZPlane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_POS_Z) == 0)
                    addFaceXPlane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_Z) == 0)
                    addFaceXPlane({x, y, z-1}, true);
            }
        }
    }

    bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(float), &m_vertices[0], GL_STATIC_DRAW));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW));
}

void Chunk::draw() {
    bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL))
}

uint8_t Chunk::getBlock(glm::ivec3 position) {
    if (
        position.x < 0 || position.x > CHUNK_SIDE-1
        || position.y < 0 || position.y > CHUNK_SIDE-1
        || position.z < 0 || position.z > CHUNK_SIDE-1
    ) {
        return 0;
    }
    return m_data[position.z * CHUNK_SIDE_POW2 + position.y * CHUNK_SIDE + position.x];
}

