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

void Chunk::add_face_x_plane(glm::ivec3 position, bool reversed) {
    unsigned int index_offset = faces++*4;
    unsigned int indices[] = {
        0u+index_offset, 1u+index_offset, 2u+index_offset,
        1u+index_offset, 3u+index_offset, 2u+index_offset,
    };
    unsigned int indices_reversed[] = {
        0u+index_offset, 2u+index_offset, 1u+index_offset,
        1u+index_offset, 2u+index_offset, 3u+index_offset,
    };

    glm::vec3 vertex_offset = position_to_world(position);
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

void Chunk::add_face_y_plane(glm::ivec3 position, bool reversed) {
    unsigned int index_offset = faces++*4;
    unsigned int indices[] = {
        0u+index_offset, 1u+index_offset, 2u+index_offset,
        1u+index_offset, 3u+index_offset, 2u+index_offset,
    };
    unsigned int indices_reversed[] = {
        0u+index_offset, 2u+index_offset, 1u+index_offset,
        1u+index_offset, 2u+index_offset, 3u+index_offset,
    };

    glm::vec3 vertex_offset = position_to_world(position);
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

void Chunk::add_face_z_plane(glm::ivec3 position, bool reversed) {
    unsigned int index_offset = faces++*4;
    unsigned int indices[] = {
        0u+index_offset, 1u+index_offset, 2u+index_offset,
        1u+index_offset, 3u+index_offset, 2u+index_offset,
    };
    unsigned int indices_reversed[] = {
        0u+index_offset, 2u+index_offset, 1u+index_offset,
        1u+index_offset, 2u+index_offset, 3u+index_offset,
    };
    
    glm::vec3 vertex_offset = position_to_world(position);
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

bool Chunk::is_position_outside(glm::ivec3 position) {
    return (
        position.x < 0 || position.x > CHUNK_SIDE-1
        || position.y < 0 || position.y > CHUNK_SIDE-1
        || position.z < 0 || position.z > CHUNK_SIDE-1
    );
}

glm::ivec3 Chunk::position_to_world(glm::ivec3 position) {
    return position + m_position * CHUNK_SIDE;
}

void Chunk::generate(World& world) {
    for (int z = 0; z < CHUNK_SIDE; z++) {
        for (int y = 0; y < CHUNK_SIDE; y++) {
            for (int x = 0; x < CHUNK_SIDE; x++) {
                uint8_t opaqueBitmask = 0x0;
                glm::ivec3 globalPos = position_to_world({x, y, z});
                int mX = globalPos.x, mY = globalPos.y, mZ = globalPos.z;
                uint8_t b =   world.get_block({mX, mY, mZ});
                uint8_t bpx = world.get_block({mX+1, mY, mZ});
                uint8_t bnx = world.get_block({mX-1, mY, mZ});
                uint8_t bpy = world.get_block({mX, mY+1, mZ});
                uint8_t bny = world.get_block({mX, mY-1, mZ});
                uint8_t bpz = world.get_block({mX, mY, mZ+1});
                uint8_t bnz = world.get_block({mX, mY, mZ-1});

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
                    add_face_y_plane({x+1, y, z}, true);
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_X) == 0)
                    add_face_y_plane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_POS_Y) == 0)
                    add_face_z_plane({x, y+1, z}, true);
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_Y) == 0)
                    add_face_z_plane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_POS_Z) == 0)
                    add_face_x_plane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_Z) == 0)
                    add_face_x_plane({x, y, z-1}, true);
            }
        }
    }

    bind();
    m_layout.enableAttribs();
    GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(float), &m_vertices[0], GL_STATIC_DRAW));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW));
}

void Chunk::draw() {
    bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL))
}

World::World() {
    for (int z = 0; z < m_view_distance; z++) {
        for (int y = 0; y < m_view_distance; y++) {
            for (int x = 0; x < m_view_distance; x++) {
                m_chunks.push_back(Chunk({x, y, z}));
            }
        }
    }
    for (int i = 0; i < m_chunks.size(); i++) {
        m_chunks[i].generate(*this);
    }
}

World::~World() { }

bool World::is_position_outside(glm::ivec3 position) {
    return (
        position.x < 0 || position.x > (m_view_distance*CHUNK_SIDE-1)
        || position.y < 0 || position.y > (m_view_distance*CHUNK_SIDE-1)
        || position.z < 0 || position.z > (m_view_distance*CHUNK_SIDE-1)
    );
}

glm::ivec3 World::get_chunk_position(glm::ivec3 position) {
    glm::ivec3 result = position / CHUNK_SIDE;
    if (position.x < 0)
        result.x--;
    if (position.y < 0)
        result.y--;
    if (position.z < 0)
        result.z--;
    return result;
}

glm::ivec3 World::get_block_position(glm::ivec3 position) {
    glm::ivec3 result = position % CHUNK_SIDE;
    if (result.x < 0)
        result.x += CHUNK_SIDE;
    if (result.y < 0)
        result.y += CHUNK_SIDE;
    if (result.z < 0)
        result.z += CHUNK_SIDE;
    return result;
}

size_t World::get_chunk_index(glm::ivec3 position) {
    glm::ivec3 chunkPos = get_chunk_position(position);
    return chunkPos.z * m_view_distance*m_view_distance + chunkPos.y * m_view_distance + chunkPos.x;
}

size_t World::get_block_index(glm::ivec3 position) {
    glm::ivec3 blockPos = get_block_position(position);
    return blockPos.z * CHUNK_SIDE_POW2 + blockPos.y * CHUNK_SIDE + blockPos.x;
}

uint8_t World::get_block(glm::ivec3 position) {
    if (is_position_outside(position))
        return 0;
    Chunk& chunk = m_chunks[get_chunk_index(position)];
    return chunk.m_data[get_block_index(position)];
}

void World::draw() {
    for (int i = 0; i < m_chunks.size(); i++) {
        m_chunks[i].draw();
    }
}

