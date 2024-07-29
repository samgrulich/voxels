#include "Chunk.h"

#include <GL/glew.h>
#include <cmath>

#include "GLCommon.h"
#include "tracy/Tracy.hpp"
#include "WorldConstants.h"


std::unordered_map<glm::ivec3, Chunk*> chunks;

Block World::getBlock(int x, int y, int z) {
    ZoneScoped;
    int cx = floorf((float)x / Consts::CHUNK_SIZE);
    int cy = floorf((float)y / Consts::CHUNK_SIZE);
    int cz = floorf((float)z / Consts::CHUNK_SIZE);

    auto chunk = chunks.find(glm::ivec3(cx, cy, cz));
    if (chunk == chunks.end()) {
        return {0, false};
    }
    
    int lx = (x >= 0) ? x % Consts::CHUNK_SIZE : Consts::CHUNK_SIZE-1 + x % Consts::CHUNK_SIZE;
    int ly = (y >= 0) ? y % Consts::CHUNK_SIZE : Consts::CHUNK_SIZE-1 + y % Consts::CHUNK_SIZE;
    int lz = (z >= 0) ? z % Consts::CHUNK_SIZE : Consts::CHUNK_SIZE-1 + z % Consts::CHUNK_SIZE;
    return chunk->second->getBlock(lx, ly, lz, x, y, z);
}

void World::setBlock(int x, int y, int z, Block block) {
    ZoneScoped;
    int cx = floorf((float)x / Consts::CHUNK_SIZE);
    int cy = floorf((float)y / Consts::CHUNK_SIZE);
    int cz = floorf((float)z / Consts::CHUNK_SIZE);

    Chunk* chunk = chunks[glm::ivec3(cx, cy, cz)];
    if (chunk == nullptr) {
        chunk = new Chunk({cx*Consts::CHUNK_SIZE, cy*Consts::CHUNK_SIZE, cz*Consts::CHUNK_SIZE});
        chunks[glm::ivec3(cx, cy, cz)] = chunk;
    }
    
    int lx = (x >= 0) ? x % Consts::CHUNK_SIZE : Consts::CHUNK_SIZE-1 + x % Consts::CHUNK_SIZE;
    int ly = (y >= 0) ? y % Consts::CHUNK_SIZE : Consts::CHUNK_SIZE-1 + y % Consts::CHUNK_SIZE;
    int lz = (z >= 0) ? z % Consts::CHUNK_SIZE : Consts::CHUNK_SIZE-1 + z % Consts::CHUNK_SIZE;
    return chunk->setBlock(lx, ly, lz, block);
}

void World::removeBlock(int x, int y, int z) {
    ZoneScoped;
    setBlock(x, y, z, {0, false});
}

Chunk::Chunk() {
    ZoneScoped;
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(5*sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    position_ = {0, 0, 0};
    blocks_ = new Block[Consts::CHUNK_SIZE_POW3]{};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
}

Chunk::Chunk(glm::ivec3 position) : Chunk() {
    position_ = position;
}

Chunk::~Chunk() {
    glDeleteBuffers(1, &ibo_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void Chunk::draw() {
    GLCall(glBindVertexArray(vao_));
    GLCall(glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, NULL));
}

void Chunk::remesh() {
    ZoneScopedN("Chunk::remesh");
    for (int z1 = 0; z1 < Consts::CHUNK_SIZE; z1++) {
        for (int y1 = 0; y1 < Consts::CHUNK_SIZE; y1++) {
            for (int x1 = 0; x1 < Consts::CHUNK_SIZE; x1++) {
                int bx = position_.x + x1;
                int by = position_.y + y1;
                int bz = position_.z + z1;
                uint8_t opaqueBitmask = 0;
                Block b = getBlock(x1, y1, z1, bx, by, bz);

                if (b.id == 0) {
                    continue;
                }

                const Block bpx = getBlock(x1+1, y1, z1, bx+1, by, bz);
                const Block bnx = getBlock(x1-1, y1, z1, bx-1, by, bz);
                const Block bpy = getBlock(x1, y1+1, z1, bx, by+1, bz);
                const Block bny = getBlock(x1, y1-1, z1, bx, by-1, bz);
                const Block bpz = getBlock(x1, y1, z1+1, bx, by, bz+1);
                const Block bnz = getBlock(x1, y1, z1-1, bx, by, bz-1);
                
                opaqueBitmask |= (!bpx.opaque && b.opaque) ? ADJACENT_BITMASK_POS_X : 0;
                opaqueBitmask |= (!bnx.opaque && b.opaque) ? ADJACENT_BITMASK_NEG_X : 0;
                opaqueBitmask |= (!bpy.opaque && b.opaque) ? ADJACENT_BITMASK_POS_Y : 0;
                opaqueBitmask |= (!bny.opaque && b.opaque) ? ADJACENT_BITMASK_NEG_Y : 0;
                opaqueBitmask |= (!bpz.opaque && b.opaque) ? ADJACENT_BITMASK_POS_Z : 0;
                opaqueBitmask |= (!bnz.opaque && b.opaque) ? ADJACENT_BITMASK_NEG_Z : 0;

                if (opaqueBitmask == 0) {
                    continue;
                }

                if (opaqueBitmask & ADJACENT_BITMASK_POS_X) {
                    addFaceXPlane(bx+1, by, bz, bx+1, by+1, bz+1, false);
                }
                if (opaqueBitmask & ADJACENT_BITMASK_NEG_X) {
                    addFaceXPlane(bx, by, bz, bx, by+1, bz+1, true);
                }
                if (opaqueBitmask & ADJACENT_BITMASK_POS_Y) {
                    addFaceYPlane(bx, by+1, bz, bx+1, by+1, bz+1, true);
                }
                if (opaqueBitmask & ADJACENT_BITMASK_NEG_Y) {
                    addFaceYPlane(bx, by, bz, bx+1, by, bz+1, false);
                }
                if (opaqueBitmask & ADJACENT_BITMASK_POS_Z) {
                    addFaceZPlane(bx, by, bz+1, bx+1, by+1, bz+1, true);
                }
                if (opaqueBitmask & ADJACENT_BITMASK_NEG_Z) {
                    addFaceZPlane(bx, by, bz, bx+1, by+1, bz, false);
                }
            }
        }
    }

    uploadMesh();
}

void Chunk::setPosition(glm::ivec3 position) {
    position_ = position;
}

const Block Chunk::getBlock(int lx, int ly, int lz, int wx, int wy, int wz) const {
    ZoneScoped;
    if (lx < 0 || ly < 0 || lz < 0 || lx >= Consts::CHUNK_SIZE || ly >= Consts::CHUNK_SIZE || lz >= Consts::CHUNK_SIZE) {
        return World::getBlock(wx, wy, wz);
    }

    return blocks_[getBlockIndex(lx, ly, lz)];
}

void Chunk::setBlock(int lx, int ly, int lz, Block block) {
    ZoneScoped;
    blocks_[getBlockIndex(lx, ly, lz)] = block;
}

void Chunk::generateChunk() {
    ZoneScopedN("Chunk::generateChunk");
    for (int lz = 0; lz < Consts::CHUNK_SIZE; lz++) {
        for (int ly = 0; ly < Consts::CHUNK_SIZE; ly++) {
            for (int lx = 0; lx < Consts::CHUNK_SIZE; lx++) {
                int x = position_.x + lx;
                int y = position_.y + ly;
                int z = position_.z + lz;
                setBlock(lx, ly, lz, generateBlock(x, y, z));
            }
        }
    }
}

void Chunk::addFaceXPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted) {
    unsigned int io = vertices_.size() / 6;
    if (inverted) {
        vertices_.insert(vertices_.end(), {
            x1, y1, z1, 0, 0, 2,
            x1, y2, z1, 0, 1, 2,
            x1, y2, z2, 1, 1, 2,
            x1, y1, z2, 1, 0, 2
        });
    } else {
        vertices_.insert(vertices_.end(), {
            x1, y1, z1, 0, 0, 3,
            x1, y1, z2, 0, 1, 3,
            x1, y2, z2, 1, 1, 3,
            x1, y2, z1, 1, 0, 3
        });
    }
    indices_.insert(indices_.end(), {
        io, io+1, io+2,
        io, io+2, io+3
    });
}

void Chunk::addFaceYPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted) {
    unsigned int io = vertices_.size() / 6;
    if (inverted) {
        vertices_.insert(vertices_.end(), {
            x1, y1, z1, 0, 0, 4,
            x2, y1, z1, 0, 1, 4,
            x2, y1, z2, 1, 1, 4,
            x1, y1, z2, 1, 0, 4
        });
    } else {
        vertices_.insert(vertices_.end(), {
            x1, y1, z1, 0, 0, 5,
            x1, y1, z2, 0, 1, 5,
            x2, y1, z2, 1, 1, 5,
            x2, y1, z1, 1, 0, 5
        });
    }
    indices_.insert(indices_.end(), {
        io, io+1, io+2,
        io, io+2, io+3
    });
}

void Chunk::addFaceZPlane(float x1, float y1, float z1, float x2, float y2, float z2, bool inverted) {
    unsigned int io = vertices_.size() / 6;
    if (inverted) {
        vertices_.insert(vertices_.end(), {
            x1, y1, z1, 0, 0, 1,
            x1, y2, z1, 0, 1, 1,
            x2, y2, z1, 1, 1, 1,
            x2, y1, z1, 1, 0, 1
        });
    } else {
        vertices_.insert(vertices_.end(), {
            x1, y1, z1, 0, 0, 0,
            x2, y1, z1, 0, 1, 0,
            x2, y2, z1, 1, 1, 0,
            x1, y2, z1, 1, 0, 0
        });
    }
    indices_.insert(indices_.end(), {
        io, io+1, io+2,
        io, io+2, io+3
    });
}

void Chunk::uploadMesh() {
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size()*sizeof(float), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size()*sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);
}

inline int Chunk::getBlockIndex(int lx, int ly, int lz) const {
    return lx + ly*Consts::CHUNK_SIZE + lz*Consts::CHUNK_SIZE_POW2;
}

inline Block generateBlock(int x, int y, int z) {
    if (y > 8)
        return Blocks::AIR;
    
    return Blocks::STONE;
}
