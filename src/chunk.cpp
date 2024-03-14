#include <cmath>
#include <cstdint>
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include <GL/glew.h>

#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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

    toSend_ = false;
    root_ = new Node;
}

Chunk::Chunk(): Chunk({0, 0, 0}) { }

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
        1.0f + vertex_offset.x,  0.0f + vertex_offset.y, 1.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x,  0.0f + vertex_offset.y, 1.0f + vertex_offset.z, 0.0, 0.0, // 1
        1.0f + vertex_offset.x,  1.0f + vertex_offset.y, 1.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x,  1.0f + vertex_offset.y, 1.0f + vertex_offset.z, 0.0, 1.0, // 3
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
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  1.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 0.0, 0.0, // 1
        0.0f + vertex_offset.x, 1.0f + vertex_offset.y,  1.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x, 1.0f + vertex_offset.y,  0.0f + vertex_offset.z, 0.0, 1.0, // 3
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
        1.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 1.0, 0.0, // 0
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  0.0f + vertex_offset.z, 0.0, 0.0, // 1
        1.0f + vertex_offset.x, 0.0f + vertex_offset.y,  1.0f + vertex_offset.z, 1.0, 1.0, // 2
        0.0f + vertex_offset.x, 0.0f + vertex_offset.y,  1.0f + vertex_offset.z, 0.0, 1.0, // 3
    };

    vertices_.insert(vertices_.end(), std::begin(vertices), std::end(vertices));
    if (!reversed)
        indices_.insert(indices_.end(), std::begin(indices), std::end(indices));
    else 
        indices_.insert(indices_.end(), std::begin(indices_reversed), std::end(indices_reversed));
}

bool Chunk::isPosOutside(glm::ivec3 position) {
    return (
           position.x < 0 || position.x > CHUNK_SIDE
        || position.y < 0 || position.y > CHUNK_SIDE
        || position.z < 0 || position.z > CHUNK_SIDE
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

void Chunk::generate() {
    glm::ivec3 point = {0, 0, 0};
    if (position_.y < 2)
        insertTo(&root_, point, 1, point, 0, 0);
}

void Chunk::mesh(World& world) {
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

    toSend_ = true;
}

void Chunk::draw() {
    bind();
    if (toSend_) {
        layout_.enableAttribs();
        GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_.size()*sizeof(float), &vertices_[0], GL_STATIC_DRAW));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size()*sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW));
    }
    GLCall(glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, NULL))
}

World::World(glm::ivec3 playerPos) 
    : lastChunk_(std::weak_ptr<Chunk>()), playerPos_(playerPos)
{
    chunks_     = std::map<glm::ivec3, std::shared_ptr<Chunk>, IVec3Comparator>();
    toGenerate_ = std::queue<std::weak_ptr<Chunk>>();
    toMesh_     = std::queue<std::weak_ptr<Chunk>>();
    active_     = std::map<glm::ivec3, std::weak_ptr<Chunk>, IVec3Comparator>();

    glm::ivec3 origin = getChunkPosition(playerPos);
    glm::ivec3 start(origin-viewDistance_);
    glm::ivec3 end  (origin+viewDistance_);
    start_ = start;
    end_ = end;

    for (int z = start.z; z <= end.z; z++) {
        for (int y = start.y; y <= end.y; y++) {
            for (int x = start.x; x <= end.x; x++) {
                glm::ivec3 position(x, y, z);
                chunks_[position] = std::make_shared<Chunk>(Chunk(position));
                toGenerate_.push((chunks_.find(position)->second));
            }
        }
    }
}

World::~World() { }

bool World::isPositionOutside(glm::ivec3 position) {
    return (
           position.x < start_.x*CHUNK_SIDE || position.x > ((end_.x+1)*CHUNK_SIDE-1)
        || position.y < start_.y*CHUNK_SIDE || position.y > ((end_.y+1)*CHUNK_SIDE-1)
        || position.z < start_.z*CHUNK_SIDE || position.z > ((end_.z+1)*CHUNK_SIDE-1)
    );
}

glm::ivec3 World::getChunkPosition(glm::ivec3 position) {
    glm::ivec3 result = position / CHUNK_SIDE;
    if (position.x < 0 && position.x % CHUNK_SIDE != 0)
        result.x--;
    if (position.y < 0 && position.y % CHUNK_SIDE != 0)
        result.y--;
    if (position.z < 0 && position.z % CHUNK_SIDE != 0)
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
    return chunkPos.z * fullViewDistance_*fullViewDistance_ + chunkPos.y * fullViewDistance_ + chunkPos.x;
}

size_t World::getBlockIndex(glm::ivec3 position) {
    glm::ivec3 blockPos = getBlockPosition(position);
    return blockPos.z * CHUNK_SIDE_POW2 + blockPos.y * CHUNK_SIDE + blockPos.x;
}
        
void World::addChunksXPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset) {
    int x = origin.x + offset;
    for (int z = origin.z-viewDistance_; z <= origin.z+viewDistance_; z++) {
        for (int y = origin.y-viewDistance_; y <= origin.y+viewDistance_; y++) {
            chunks.push_back({x, y, z});
        }
    }
}

void World::addChunksYPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset) {
    int y = origin.y + offset;
    for (int z = origin.z-viewDistance_; z <= origin.z+viewDistance_; z++) {
        for (int x = origin.x-viewDistance_; x <= origin.x+viewDistance_; x++) {
            chunks.push_back({x, y, z});
        }
    }
}

void World::addChunksZPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset) {
    int z = origin.z + offset;
    for (int y = origin.y-viewDistance_; y <= origin.y+viewDistance_; y++) {
        for (int x = origin.x-viewDistance_; x <= origin.x+viewDistance_; x++) {
            chunks.push_back({x, y, z});
        }
    }
}

void World::loadChunks(std::vector<glm::ivec3>& chunkPositions) {
    for (glm::ivec3 chunkPos : chunkPositions) {
        // todo
        chunks_[chunkPos] = std::make_shared<Chunk>(Chunk(chunkPos));
        toGenerate_.push(chunks_.find(chunkPos)->second);
    }
}

void World::unloadChunks(std::vector<glm::ivec3>& chunkPositions) {
    for (glm::ivec3 chunkPos : chunkPositions) {
        // save chunk
        chunks_.erase(chunkPos);
    }
}

uint8_t World::getBlock(glm::ivec3 position) {
    if (isPositionOutside(position))
        return 0;
    glm::ivec3 chunkPos = getChunkPosition(position);
    std::shared_ptr<Chunk> chunk;
    if (!(chunk = lastChunk_.lock())
        || chunk->position_ != chunkPos
    ) {
        chunk = chunks_[chunkPos];
        lastChunk_ = chunk;
    }
    return chunk->getBlock(getBlockPosition(position));
}

void World::generateChunks() {
    if (toGenerate_.size() == 0)
        return;
    std::list<std::weak_ptr<Chunk>> generatedChunks;
    {
        std::lock_guard<std::mutex> g1(mtxToGenerate_);
        int size = toGenerate_.size();
        for (int i = 0; i < size; i++) {
            if(std::shared_ptr<Chunk> chunk = toGenerate_.front().lock()) {
                chunk->generate();
                generatedChunks.push_back(chunk);
            }
            toGenerate_.pop();
        }
    }
    if (generatedChunks.size() == 0) 
        return;
    std::lock_guard<std::mutex> g2(mtxToMesh_);
    for (const auto &chunk : generatedChunks) {
        toMesh_.push(chunk);
    }
}

void World::meshChunks() {
    if (toMesh_.size() == 0)
        return;
    std::list<std::weak_ptr<Chunk>> meshedChunks;
    {
        std::lock_guard<std::mutex> g1(mtxToMesh_);
        int size = toMesh_.size();
        for (int i = 0; i < size; i++) {
            if (std::shared_ptr<Chunk> chunk = toMesh_.front().lock()) {
                chunk->mesh(*this);
                meshedChunks.push_back(chunk);
            }
            toMesh_.pop();
        }
    }
    if (meshedChunks.size() == 0)
        return;
    std::lock_guard<std::mutex> g2(mtxActive_);
    for (const auto &chunk : meshedChunks) {
        active_[chunk.lock()->position_] = chunk;
    }
}

void World::updateRegion(glm::ivec3 camPos) {
    // chunk aligned positions (world pos % chunk side)
    glm::ivec3 lastChunkPos = getChunkPosition(playerPos_);
    glm::ivec3 currChunkPos = getChunkPosition(camPos);
    if (getChunkPosition(camPos) != getChunkPosition(playerPos_)) {
        glm::ivec3 delta = currChunkPos - lastChunkPos;
        glm::ivec3 deltaAbs = glm::abs(delta);
        glm::ivec3 deltaSign = glm::sign(delta);
        glm::ivec3 loadOffset = viewDistance_/2 * deltaSign; 
        glm::ivec3 unloadOffset = -loadOffset;

        std::vector<glm::ivec3> toLoad;
        std::vector<glm::ivec3> toUnload;

        for (int xd = 1; xd <= deltaAbs.x; xd++) {
            int immOffset = xd*deltaSign.x;
            addChunksXPlane(toLoad, currChunkPos, loadOffset.x+immOffset);
            addChunksXPlane(toUnload, lastChunkPos, unloadOffset.x-immOffset);
        }
        for (int yd = 1; yd <= deltaAbs.y; yd++) {
            int immOffset = yd*deltaSign.y;
            addChunksYPlane(toLoad, currChunkPos, loadOffset.y+immOffset);
            addChunksYPlane(toUnload, lastChunkPos, unloadOffset.y-immOffset);
        }
        for (int zd = 1; zd <= deltaAbs.z; zd++) {
            int immOffset = zd*deltaSign.z;
            addChunksZPlane(toLoad, currChunkPos, loadOffset.z+immOffset);
            addChunksZPlane(toUnload, lastChunkPos, unloadOffset.z-immOffset);
        }
        loadChunks(toLoad);
        unloadChunks(toUnload);
        start_ = currChunkPos - viewDistance_;
        end_ = currChunkPos + viewDistance_;
        playerPos_ = camPos;
    }
}

void World::draw() {
    std::vector<glm::ivec3> toRemove;
    for (const auto &[position, chunk] : active_) {
        if (std::shared_ptr<Chunk> chunk_ptr = chunk.lock()) {
            chunk_ptr->draw();
        } else {
            toRemove.push_back(position);
        }
    }
    for (glm::ivec3 position : toRemove) {
        active_.erase(position);
    }
}

