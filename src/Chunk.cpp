#include "Chunk.h"

ChunkMetadata::ChunkMetadata() 
    : toGenerate_(true), toMesh_(false), active_(false)
{
    position_ = {0, 0, 0};
    offset_ = {0, 0, 0};
    chunk_ = nullptr;
}

ChunkMetadata::ChunkMetadata(Chunk& chunk) {
    toGenerate_ = true;
    toMesh_ = false;
    active_ = false;
    position_ = chunk.position_;
    offset_ = chunk.offset_;
    chunk_  = nullptr;
}

ChunkMetadata::ChunkMetadata(std::weak_ptr<Chunk> chunk) {
    ChunkMetadata& chunkMeta = chunk.lock()->metadata_;
    toGenerate_ = chunkMeta.toGenerate_;
    toMesh_ = chunkMeta.toMesh_;
    active_ = chunkMeta.active_;
    position_ = chunkMeta.position_;
    offset_ = chunkMeta.offset_;
    chunk = std::make_shared<Chunk>(chunk);
}

ChunkMetadata::ChunkMetadata(glm::ivec3 position) {
    auto it = chunks_.find(position);
    if (it != chunks_.end()) {
        auto chunk = it->second;
        ChunkMetadata& chunkMeta = chunk->metadata_;
        toGenerate_ = chunkMeta.toGenerate_;
        toMesh_ = chunkMeta.toMesh_;
        active_ = chunkMeta.active_;
        position_ = chunkMeta.position_;
        offset_ = chunkMeta.offset_;
        chunk = std::make_shared<Chunk>(chunk);
    }
    toGenerate_ = true;
    toMesh_ = false;
    active_ = false;
    position_ = position;
    offset_ = position * (const int)World::CHUNK_SIZE;
    chunk_  = nullptr;
}

void ChunkMetadata::setToGenerate() {
    toGenerate_ = true;
    toMesh_ = false;
    active_ = false;
}

void ChunkMetadata::setToMesh() {
    toGenerate_ = false;
    toMesh_ = true;
    active_ = false;
}

void ChunkMetadata::setToActive() {
    toGenerate_ = false;
    toMesh_ = false;
    active_ = true;
}

void ChunkMetadata::setToUnload() {
    toGenerate_ = false;
    toMesh_ = false;
    active_ = false;
}

bool ChunkMetadata::isGeneratable() {
    return toGenerate_;
}

bool ChunkMetadata::isMeshable() {
    return toMesh_;
}

bool ChunkMetadata::isActive() {
    return active_;
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

}
