#include "Chunk.h"
#include "GLCommon.h"

ChunkMetadata::ChunkMetadata() 
    : toGenerate_(true), toMesh_(false), toUpload_(false), isActive_(false)
{
    position_ = {0, 0, 0};
    offset_ = {0, 0, 0};
    chunk_ = nullptr;
}

ChunkMetadata::ChunkMetadata(Chunk& chunk) 
    : toGenerate_(true), toMesh_(false), toUpload_(false), isActive_(false)
{
    position_ = chunk.position_;
    offset_ = chunk.offset_;
    chunk_  = nullptr;
}

ChunkMetadata::ChunkMetadata(std::weak_ptr<Chunk> chunk)  {
    ChunkMetadata& chunkMeta = chunk.lock()->metadata_;
    toGenerate_ = chunkMeta.toGenerate_;
    toMesh_ = chunkMeta.toMesh_;
    toUpload_ = chunkMeta.toUpload_;
    isActive_ = chunkMeta.isActive_;
    position_ = chunkMeta.position_;
    offset_ = chunkMeta.offset_;
    chunk = std::make_shared<Chunk>(chunk);
}

ChunkMetadata::ChunkMetadata(glm::ivec3 position) 
    : toGenerate_(true), toMesh_(false), toUpload_(false), isActive_(false), 
    position_(position), chunk_(nullptr)
{
    offset_ = position * (const int)World::CHUNK_SIZE;
    auto it = chunks_.find(position);
    if (it != chunks_.end()) {
        auto chunk = it->second;
        ChunkMetadata& chunkMeta = chunk->metadata_;
        toGenerate_ = chunkMeta.toGenerate_;
        toMesh_ = chunkMeta.toMesh_;
        toUpload_ = chunkMeta.toUpload_;
        isActive_ = chunkMeta.isActive_;
        position_ = chunkMeta.position_;
        offset_ = chunkMeta.offset_;
        chunk = std::make_shared<Chunk>(chunk);
    }
}

void ChunkMetadata::setToGenerate() {
    toGenerate_ = true;
    toMesh_ = false;
    toUpload_ = false;
    isActive_ = false;
}

void ChunkMetadata::setToMesh() {
    toGenerate_ = false;
    toMesh_ = true;
    toUpload_ = false;
    isActive_ = false;
}

void ChunkMetadata::setToActive() {
    toGenerate_ = false;
    toMesh_ = false;
    toUpload_ = false;
    isActive_ = true;
}

void ChunkMetadata::setToUnload() {
    toGenerate_ = false;
    toMesh_ = false;
    toUpload_ = false;
    isActive_ = false;
}

bool ChunkMetadata::toGenerate() {
    return toGenerate_;
}

bool ChunkMetadata::toMesh() {
    return toMesh_;
}

bool ChunkMetadata::toUpload() {
    return toUpload_;
}

bool ChunkMetadata::isActive() {
    return isActive_;
}

std::weak_ptr<Chunk> ChunkMetadata::getWeak() {
    return chunk_;
}

std::shared_ptr<Chunk> ChunkMetadata::getShared() {
    return chunk_;
}

glm::ivec3 ChunkMetadata::position() {
    return position_;
}

Chunk::Chunk(GLint posX, GLint posY, GLint posZ)
    : opaqueVAO_(), opaqueVBO_()
{
    opaqueVAO_.bind();
    opaqueVBO_.bind();
    opaqueVAO_.linkAttrib(opaqueVBO_, 0, 1, 1, 0);
    opaqueVBO_.unbind();
    opaqueVAO_.unbind();

    offset_ = {posX, posY, posZ};
    offset_ *= World::CHUNK_SIZE;
    position_ = {posX, posY, posZ};
    position_ *= World::CHUNK_SIZE;

    metadata_ = ChunkMetadata(*this);
}

Chunk::~Chunk() {
    opaqueVAO_.remove();
    opaqueVBO_.remove();
    delete[] &opaqueVertices_;
    delete[] &blocks_;
}

void Chunk::generate(GLuint seed) {
    // todo
}

void Chunk::generateFace(Block& block, GLuint faceIndex) {
    uint32_t posX = block.position_.x; // 6 bits, 0-63
    uint32_t posY = block.position_.y; // 6 bits, 0-63
    uint32_t posZ = block.position_.z; // 6 bits, 0-63
    uint32_t faceId = faceIndex;       // 3 bits, 0-7
    // vertexId                        // 3 bits, 0-7
    uint32_t id = block.id;            // 5 bits, 0-31
    // Ambient Occlusion Enabled       // (1 Bit,  0-1)

    uint32_t packedVertexOne;
    uint32_t packedVertexTwo;
    uint32_t packedVertexThree;
    uint32_t packedVertexFour;
    uint32_t packedVertexFive;
    uint32_t packedVertexSix;

    switch(faceId) {
        default: 
            packedVertexOne   = (posX | posY << 6 | posZ << 12 | faceId << 18 | 0 << 21 | id << 24);
            packedVertexTwo   = (posX | posY << 6 | posZ << 12 | faceId << 18 | 1 << 21 | id << 24);
            packedVertexThree = (posX | posY << 6 | posZ << 12 | faceId << 18 | 2 << 21 | id << 24);
            packedVertexFour  = (posX | posY << 6 | posZ << 12 | faceId << 18 | 3 << 21 | id << 24);
            packedVertexFive  = (posX | posY << 6 | posZ << 12 | faceId << 18 | 4 << 21 | id << 24);
            packedVertexSix   = (posX | posY << 6 | posZ << 12 | faceId << 18 | 5 << 21 | id << 24);
            break;
    }

    // if is not transparent
    opaqueVertices_.push_back(packedVertexOne);
    opaqueVertices_.push_back(packedVertexTwo);
    opaqueVertices_.push_back(packedVertexThree);
    opaqueVertices_.push_back(packedVertexFour);
    opaqueVertices_.push_back(packedVertexFive);
}

void Chunk::generateMesh() {
    for (int z = 0; z < World::CHUNK_SIZE; z++)
    for (int y = 0; y < World::CHUNK_SIZE; y++)
    for (int x = 0; x < World::CHUNK_SIZE; x++) {
        Block block = getBlock(x, y, z);
        if (block.id != World::BlockIDs::air) {
            // we want to render the face if an adjancent block
            // is air. 
            // if statements are checking for air
            // else statements check chunk borders
            if (y < World::CHUNK_SIZE - 1) {
                if (isBlockTransparent(x, y+1, z)) {
                    generateFace(block, BlockFace::Top);
                }
            } else {
                // space for world height checking
                if (chunks_[{position_.x, position_.y+1, position_.z}]->isBlockTransparent(x, 0, z)) {
                    generateFace(block, BlockFace::Top);
                }
            }

            if (y != 0) {
                if (isBlockTransparent(x, y-1, z)) {
                    generateFace(block, BlockFace::Bottom);
                }
            } else {
                // space for world height checking
                if (chunks_[{position_.x, position_.y-1, position_.z}]->isBlockTransparent(x, World::CHUNK_LAST_IDX, z)) {
                    generateFace(block, BlockFace::Bottom);
                }
            }
            
            // x and z
            if (x < World::CHUNK_SIZE - 1) {
                if (isBlockTransparent(x+1, y, z)) {
                    generateFace(block, BlockFace::Right);
                }
            } else {
                if (chunks_[{position_.x+1, position_.y, position_.z}]->isBlockTransparent(0, y, z)) {
                    generateFace(block, BlockFace::Right);
                }
            }
            
            if (x != 0) {
                if (isBlockTransparent(x-1, y, z)) {
                    generateFace(block, BlockFace::Left);
                }
            } else {
                // space for world height checking
                if (chunks_[{position_.x-1, position_.y, position_.z}]->isBlockTransparent(World::CHUNK_LAST_IDX, y, z)) {
                    generateFace(block, BlockFace::Left);
                }
            }

            if (z < World::CHUNK_SIZE - 1) {
                if (isBlockTransparent(x, y, z+1)) {
                    generateFace(block, BlockFace::Front);
                }
            } else {
                if (chunks_[{position_.x, position_.y, position_.z+1}]->isBlockTransparent(x, y, 0)) {
                    generateFace(block, BlockFace::Front);
                }
            }
            
            if (z != 0) {
                if (isBlockTransparent(x, y, z-1)) {
                    generateFace(block, BlockFace::Back);
                }
            } else {
                // space for world height checking
                if (chunks_[{position_.x, position_.y, position_.z-1}]->isBlockTransparent(x, y, World::CHUNK_LAST_IDX)) {
                    generateFace(block, BlockFace::Back);
                }
            }
        }
    }
}

void Chunk::remesh() {
    opaqueVertices_.clear();

    metadata_.setToMesh();
}

void Chunk::draw(ShaderProgram shaderProgram, bool renderOpaque) {
    if (metadata_.toUpload()) { // if chunk is meshed and wait for upload
        // upload the mesh to the GPU
        opaqueVAO_.bind();
        opaqueVBO_.set(opaqueVertices_);
        opaqueVAO_.unbind();
        metadata_.setToActive();
    }

    if (metadata_.isActive()) {
        shaderProgram.set("u_chunkOffset", offset_);
        if (renderOpaque) {
            opaqueVAO_.bind();
            GLCall(glDrawArrays(GL_TRIANGLES, 0, opaqueVertices_.size()));
            opaqueVAO_.unbind();
        }
    }
}

inline void Chunk::setBlock(Block block) {
    blocks_[block.position_.x + block.position_.z*World::CHUNK_SIZE + block.position_.y*World::CHUNK_SIZE_POW2] = block;
}

inline Block& Chunk::getBlock(int x, int y, int z) {
    return blocks_[x + z*World::CHUNK_SIZE + y*World::CHUNK_SIZE_POW2];
}

inline Block& Chunk::getBlock(glm::ivec3 pos) {
    return blocks_[pos.x + pos.z*World::CHUNK_SIZE + pos.y*World::CHUNK_SIZE_POW2];
}

// ? set type
// ? set position

inline bool Chunk::isBlockTransparent(int x, int y, int z) {
    return blocks_[x + z*World::CHUNK_SIZE + y*World::CHUNK_SIZE_POW2].isTransparent;
}
