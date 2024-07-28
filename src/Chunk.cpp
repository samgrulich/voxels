#include "Chunk.h"

#include <GL/glew.h>

#include "tracy/Tracy.hpp"
#include "WorldConstants.h"


Block World::getBlock(int x, int y, int z) {
    ZoneScoped;
    return blocks[glm::vec3(x, y, z)];
}

Block World::getBlock(glm::vec3 pos) {
    ZoneScoped;
    return blocks[pos];
}

Block World::setBlock(int x, int y, int z, Block block) {
    ZoneScoped;
    Block oldBlock = blocks[glm::vec3(x, y, z)];
    blocks[glm::vec3(x, y, z)] = block;
    return oldBlock;
}

Block World::setBlock(glm::vec3 pos, Block block) {
    ZoneScoped;
    Block oldBlock = blocks[pos];
    blocks[pos] = block;
    return oldBlock;
}

Block World::removeBlock(int x, int y, int z) {
    ZoneScoped;
    Block oldBlock = blocks[glm::vec3(x, y, z)];
    blocks.erase(glm::vec3(x, y, z));
    return oldBlock;
}

Chunk::Chunk() {
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

void Chunk::setPosition(glm::ivec3 position) {
    position_ = position;
}

void Chunk::draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, NULL);
}

void Chunk::remesh() {
    ZoneScopedN("Chunk::remesh");
    for (int z1 = 0; z1 < Consts::CHUNK_SIZE; z1++) {
        for (int y1 = 0; y1 < Consts::CHUNK_SIZE; y1++) {
            for (int x1 = 0; x1 < Consts::CHUNK_SIZE; x1++) {
                ZoneScopedN("Chunk::remesh::block");
                int bx = position_.x + x1;
                int by = position_.y + y1;
                int bz = position_.z + z1;
                Block b = World::getBlock(bx, by, bz);
                uint8_t opaqueBitmask = 0;

                if (b.id == 0) {
                    continue;
                }

                const Block bpx = World::getBlock(position_.x + x1 + 1, position_.y + y1, position_.z + z1);
                const Block bnx = World::getBlock(position_.x + x1 - 1, position_.y + y1, position_.z + z1);
                const Block bpy = World::getBlock(position_.x + x1, position_.y + y1 + 1, position_.z + z1);
                const Block bny = World::getBlock(position_.x + x1, position_.y + y1 - 1, position_.z + z1);
                const Block bpz = World::getBlock(position_.x + x1, position_.y + y1, position_.z + z1 + 1);
                const Block bnz = World::getBlock(position_.x + x1, position_.y + y1, position_.z + z1 - 1);
                
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


