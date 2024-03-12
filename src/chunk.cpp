#include <cmath>
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
    : position_(position)
{
    vertices_ = std::vector<float>(0, DEFAULT_VERTICES_LEN);
    indices_ = std::vector<unsigned int>(0, DEFAULT_INDICES_LEN);

    GLCall(glGenVertexArrays(1, &VAO_));
    GLCall(glBindVertexArray(VAO_));
    
    GLCall(glGenBuffers(1, &VB_));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VB_));

    layout_ = Vertex::getLayout();

    GLCall(glGenBuffers(1, &IB_));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB_));
    
    root_ = new Node;
    
    glm::ivec3 point = {0, 0, 0};
    if (position.y < 2)
        insertTo(&root_, point, 1, point, 0, 0);
}

Chunk::Chunk() {
    Chunk({0, 0, 0});
}

Chunk::~Chunk() {
    GLCall(glDeleteBuffers(1, &VAO_));
    GLCall(glDeleteBuffers(1, &VB_));
    GLCall(glDeleteBuffers(1, &IB_));
}

void Chunk::bind() {
    GLCall(glBindVertexArray(VAO_));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VB_));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB_));
}

void Chunk::setBlock(glm::ivec3 position, uint8_t value) {
    insert(&root_, position, value, position, 0);
}

uint8_t Chunk::getBlock(glm::ivec3 position) {
    return get(&root_, position, position, 0);
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

    glm::vec3 vertex_offset = posToWorld(position);
    float vertices[] = {
        1.0f + vertex_offset.x,  0.0f + vertex_offset.y, 0.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x,  0.0f + vertex_offset.y, 0.0f + vertex_offset.z, 0.0, 0.0, // 1
        1.0f + vertex_offset.x,  1.0f + vertex_offset.y, 0.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x,  1.0f + vertex_offset.y, 0.0f + vertex_offset.z, 0.0, 1.0, // 3
    };

    vertices_.insert(vertices_.end(), std::begin(vertices), std::end(vertices));
    if (!reversed)
        indices_.insert(indices_.end(), std::begin(indices), std::end(indices));
    else 
        indices_.insert(indices_.end(), std::begin(indices_reversed), std::end(indices_reversed));
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

    glm::vec3 vertex_offset = posToWorld(position);
    float vertices[] = {
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y, -1.0f + vertex_offset.z, 0.0, 0.0, // 1
        0.0f + vertex_offset.x, 1.0f + vertex_offset.y,  0.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x, 1.0f + vertex_offset.y, -1.0f + vertex_offset.z, 0.0, 1.0, // 3
    };

    vertices_.insert(vertices_.end(), std::begin(vertices), std::end(vertices));
    if (!reversed)
        indices_.insert(indices_.end(), std::begin(indices), std::end(indices));
    else 
        indices_.insert(indices_.end(), std::begin(indices_reversed), std::end(indices_reversed));
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
    
    glm::vec3 vertex_offset = posToWorld(position);
    float vertices[] = {
        1.0f + vertex_offset.x, 0.0f + vertex_offset.y, -1.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y, -1.0f + vertex_offset.z, 0.0, 0.0, // 1
        1.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 0.0, 1.0, // 3
    };

    vertices_.insert(vertices_.end(), std::begin(vertices), std::end(vertices));
    if (!reversed)
        indices_.insert(indices_.end(), std::begin(indices), std::end(indices));
    else 
        indices_.insert(indices_.end(), std::begin(indices_reversed), std::end(indices_reversed));
}

bool Chunk::isPosOutside(glm::ivec3 position) {
    return (
        position.x < 0 || position.x > CHUNK_SIDE-1
        || position.y < 0 || position.y > CHUNK_SIDE-1
        || position.z < 0 || position.z > CHUNK_SIDE-1
    );
}

glm::ivec3 Chunk::posToWorld(glm::ivec3 position) {
    return position + position_ * CHUNK_SIDE;
}

void Chunk::insert(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth) {
    insertTo(node, point, value, position, depth, depth_);
}

void Chunk::insertTo(Node** node, glm::ivec3 point, uint8_t value, glm::ivec3 position, int depth, int maxDepth) {
    if (*node == nullptr) {
        *node = new Node;
    }

    (*node)->data = value;
    if (depth == maxDepth) {
        (*node)->isLeaf = true;
        return;
    }

    float size = size_ / std::exp2(depth);

    glm::ivec3 childPos = {
        point.x >= (size * position.x) + (size / 2.f),
        point.y >= (size * position.y) + (size / 2.f),
        point.z >= (size * position.z) + (size / 2.f),
    };

    int childIndex = (childPos.x << 0) | (childPos.y << 1) | (childPos.z << 2);

    position = {
        (position.x << 1) | childPos.x,
        (position.y << 1) | childPos.y,
        (position.z << 1) | childPos.z,
    };

    insert(&(*node)->children[childIndex], point, value, position, ++depth);
}

uint8_t Chunk::get(Node** node, glm::ivec3 point, glm::ivec3 position, int depth) {
    if (*node == nullptr) {
        return 0;
    }

    if ((*node)->isLeaf) {
        return (*node)->data;
    }

    float size = size_ / std::exp2(depth);

    glm::ivec3 childPos = {
        point.x >= (size * position.x) + (size / 2.f),
        point.y >= (size * position.y) + (size / 2.f),
        point.z >= (size * position.z) + (size / 2.f),
    };

    int childIndex = (childPos.x << 0) | (childPos.y << 1) | (childPos.z << 2);

    position = {
        (position.x << 1) | childPos.x,
        (position.y << 1) | childPos.y,
        (position.z << 1) | childPos.z,
    };

    return get(&(*node)->children[childIndex], point, position, ++depth);
}

void Chunk::generate(World& world) {
    for (int z = 0; z < CHUNK_SIDE; z++) {
        for (int y = 0; y < CHUNK_SIDE; y++) {
            for (int x = 0; x < CHUNK_SIDE; x++) {
                uint8_t opaqueBitmask = 0x0;
                glm::ivec3 globalPos = posToWorld({x, y, z});
                int mX = globalPos.x, mY = globalPos.y, mZ = globalPos.z;
                uint8_t b =   world.getBlock({mX, mY, mZ});
                uint8_t bpx = world.getBlock({mX+1, mY, mZ});
                uint8_t bnx = world.getBlock({mX-1, mY, mZ});
                uint8_t bpy = world.getBlock({mX, mY+1, mZ});
                uint8_t bny = world.getBlock({mX, mY-1, mZ});
                uint8_t bpz = world.getBlock({mX, mY, mZ+1});
                uint8_t bnz = world.getBlock({mX, mY, mZ-1});

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
                    addFaceZPlane({x+1, y, z}, true);
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_X) == 0)
                    addFaceZPlane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_POS_Y) == 0)
                    addFaceYPlane({x, y+1, z}, true); // X
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_Y) == 0)
                    addFaceYPlane({x, y, z}, false);
                if ((opaqueBitmask & ADJACENT_BITMASK_POS_Z) == 0)
                    addFaceXPlane({x, y, z}, false); // Z
                if ((opaqueBitmask & ADJACENT_BITMASK_NEG_Z) == 0)
                    addFaceXPlane({x, y, z-1}, true);
            }
        }
    }

    bind();
    layout_.enableAttribs();
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_.size()*sizeof(float), &vertices_[0], GL_STATIC_DRAW));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size()*sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW));
}

void Chunk::draw() {
    bind();
    GLCall(glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, NULL))
}

World::World() 
    :lastChunk_(nullptr)
{
    chunks_ = std::map<glm::ivec3, Chunk, IVec3Comparator>();
    generated_ = std::vector<Chunk*>();
    toGenerate_ = std::queue<Chunk*>();

    glm::ivec3 start(-viewDistance_/2);
    glm::ivec3 end = -start;
    start_ = start;
    end_ = end;

    for (int z = start.z; z < end.z; z++) {
        for (int y = start.y; y < end.y; y++) {
            for (int x = start.x; x < end.x; x++) {
                glm::ivec3 position(x, y, z);
                chunks_[position] = Chunk(position);
                toGenerate_.push(&(chunks_.find(position)->second));
            }
        }
    }
}

World::~World() { }

bool World::isPositionOutside(glm::ivec3 position) {
    return (
        position.x < start_.x*CHUNK_SIDE+1 || position.x > (end_.x*CHUNK_SIDE-1)
        || position.y < start_.y*CHUNK_SIDE+1 || position.y > (end_.y*CHUNK_SIDE-1)
        || position.z < start_.z*CHUNK_SIDE+1 || position.z > (end_.z*CHUNK_SIDE-1)
    );
}

glm::ivec3 World::getChunkPosition(glm::ivec3 position) {
    glm::ivec3 result = position / CHUNK_SIDE;
    if (position.x < 0)
        result.x--;
    if (position.y < 0)
        result.y--;
    if (position.z < 0)
        result.z--;
    return result;
}

glm::ivec3 World::getBlockPosition(glm::ivec3 position) {
    glm::ivec3 result = position % CHUNK_SIDE;
    if (result.x < 0)
        result.x += CHUNK_SIDE;
    if (result.y < 0)
        result.y += CHUNK_SIDE;
    if (result.z < 0)
        result.z += CHUNK_SIDE;
    return result;
}

size_t World::getChunkIndex(glm::ivec3 position) {
    glm::ivec3 chunkPos = getChunkPosition(position);
    return chunkPos.z * viewDistance_*viewDistance_ + chunkPos.y * viewDistance_ + chunkPos.x;
}

size_t World::getBlockIndex(glm::ivec3 position) {
    glm::ivec3 blockPos = getBlockPosition(position);
    return blockPos.z * CHUNK_SIDE_POW2 + blockPos.y * CHUNK_SIDE + blockPos.x;
}

uint8_t World::getBlock(glm::ivec3 position) {
    if (isPositionOutside(position))
        return 0;
    glm::ivec3 chunk_position = getChunkPosition(position);
    Chunk* chunk;
    if (lastChunk_ != nullptr 
        && lastChunk_->position_.x == chunk_position.x
        && lastChunk_->position_.y == chunk_position.y
        && lastChunk_->position_.z == chunk_position.z
    ) {
        chunk = lastChunk_;
    } else {
        chunk = &chunks_[chunk_position];
        lastChunk_ = chunk;
    }
    return chunk->getBlock(getBlockPosition(position));
}

void World::generateChunk() {
    if (toGenerate_.size() == 0)
        return;
    Chunk* chunk = toGenerate_.front();
    chunk->generate(*this);
    generated_.push_back(chunk);
    toGenerate_.pop();
}

void World::draw() {
    for (int i = 0; i < generated_.size(); i++) {
        generated_[i]->draw();
    }
}

